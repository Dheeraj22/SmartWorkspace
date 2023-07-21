import boto3
import json
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
from boto3.dynamodb.conditions import Key, Attr

print('Loading function')
dynamo = boto3.client('dynamodb')
dynamodb = boto3.resource('dynamodb', endpoint_url="http://localhost:8000")

DYNAMODB_TABLE = "smart_workspace"

table = None

# For certificate based connection
myMQTTClient = AWSIoTMQTTClient("myClientID")
# For Websocket connection
# myMQTTClient = AWSIoTMQTTClient("myClientID", useWebsocket=True)
# Configurations
# For TLS mutual authentication
myMQTTClient.configureEndpoint("a2wez1ijy9ju6g-ats.iot.us-east-1.amazonaws.com", 8883)
# For Websocket
# myMQTTClient.configureEndpoint("YOUR.ENDPOINT", 443)
# For TLS mutual authentication with TLS ALPN extension
# myMQTTClient.configureEndpoint("YOUR.ENDPOINT", 443)
myMQTTClient.configureCredentials("certs/AmazonRootCA1.pem", "certs/c68a8ff5c4-private.pem.key", "certs/c68a8ff5c4-certificate.pem.crt")
# For Websocket, we only need to configure the root CA
# myMQTTClient.configureCredentials("YOUR/ROOT/CA/PATH")
myMQTTClient.configureOfflinePublishQueueing(-1)  # Infinite offline Publish queueing
myMQTTClient.configureDrainingFrequency(2)  # Draining: 2 Hz
myMQTTClient.configureConnectDisconnectTimeout(10)  # 10 sec
myMQTTClient.configureMQTTOperationTimeout(5)  # 5 sec


def respond(err, res=None):
    return {
        'statusCode': '400' if err else '200',
        'body': err.message if err else json.dumps(res),
        'headers': {
            'Content-Type': 'application/json',
        },
    }

def lambda_handler(event, context):
    '''Demonstrates a simple HTTP endpoint using API Gateway. You have full
    access to the request and response payload, including headers and
    status code.

    To scan a DynamoDB table, make a GET request with the TableName as a
    query string parameter. To put, update, or delete an item, make a POST,
    PUT, or DELETE request respectively, passing in the payload to the
    DynamoDB API as a JSON body.
    '''
    table = dynamodb.Table(DYNAMODB_TABLE)

    print(event)

    print("Received data: " + json.dumps(event, indent=2))

    message = json.dumps(event)

    if "rfid" in message:
        message = json.loads(message)
        print("Fetching user preferences from Database")
        rfid_number = message["rfid"]
        print("RFID Number: ", rfid_number)
        employee_record = search_employee(rfid_number)
        employee_prefs = get_user_prefs(employee_record)
        print("Employee Prefs: ", employee_prefs)
        publish_user_prefs(employee_prefs)
    elif "number" in message:
        print("Updating cubicle data")
        message = json.loads(message)
        rfid_number = message["number"]
        light = message["temperature"]
        fan = message["humidity"]
        co2 = message["co2"]
        employee_record = search_employee(rfid_number)
        employee_id = get_employee_id(employee_record)
        update_user_prefs(employee_id, light, fan)
    elif "eventName" in message:
        message = json.loads(message)
        print("Dynamo DB Database modified by user!")
        employee_records = message["Records"][0]["dynamodb"]["NewImage"]
        payload = create_app_update_payload(employee_records)
        publish_updated_prefs(payload)
        print("Sending payload to topic smartworkspace/app: ", payload)


def get_all_records():
    response = dynamo.scan(
        TableName=DYNAMODB_TABLE,
        AttributesToGet=[
            'Employee_ID',
            'Username',
            'RFID_Number'
        ],
        Select='SPECIFIC_ATTRIBUTES',
    )

    return response


def search_employee(rfid_number):

    response = dynamo.scan(
        TableName=DYNAMODB_TABLE,
        Select='ALL_ATTRIBUTES',
        FilterExpression='#RFID = :RFID_Number',
        ExpressionAttributeNames={
            '#RFID': 'RFID_Number'
        },
        ExpressionAttributeValues={
            ':RFID_Number': {'N': str(rfid_number)},
        },
    )

    print(response)

    return response


def get_employee_id(record):

    record = json.dumps(record)
    data = json.loads(record)
    user_pref = data["Items"][0]
    employee_id = user_pref['Employee_ID']

    return employee_id


def get_user_prefs(record):

    record = json.dumps(record)
    data = json.loads(record)
    user_pref = data["Items"][0]
    light = user_pref['Light']
    rfid = user_pref['RFID_Number']
    fan = user_pref['Fan']
    user_name = user_pref['Username']

    payload = {}
    payload['light'] = light
    payload['fan'] = fan
    payload['name'] = user_name
    payload['rfid'] = rfid
    json_payload = json.dumps(payload)

    return json_payload


def create_app_update_payload(record):

    record = json.dumps(record)
    record = json.loads(record)
    light = record['Light']['N']
    rfid = record['RFID_Number']['N']
    fan = record['Fan']['N']

    payload = {}
    payload['light'] = light
    payload['fan'] = fan
    payload['rfid'] = rfid
    json_payload = json.dumps(payload)

    return json_payload


def update_user_prefs(employee_id, light, fan):

    dynamo.update_item(
        TableName=DYNAMODB_TABLE,
        Key={
            'Employee_ID': employee_id,
        },
        UpdateExpression='SET Light=:light, Fan=:fan',
        ExpressionAttributeValues={
            ':light': {'N': str(light)},
            ':fan': {'N': str(fan)}
        }
    )


def publish_updated_prefs(user_prefs):

    myMQTTClient.connect()
    myMQTTClient.publish("smartworkspace/app", user_prefs, 1)
    myMQTTClient.disconnect()


def publish_user_prefs(user_prefs):

    myMQTTClient.connect()
    myMQTTClient.publish("smartworkspace/user", user_prefs, 1)
    myMQTTClient.disconnect()