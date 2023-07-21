package com.example.smartworkspace;

import android.content.Context;
import android.util.Log;

import com.amazonaws.auth.CognitoCachingCredentialsProvider;
import com.amazonaws.mobileconnectors.dynamodbv2.document.Table;
import com.amazonaws.mobileconnectors.dynamodbv2.document.UpdateItemOperationConfig;
import com.amazonaws.mobileconnectors.dynamodbv2.document.datatype.Document;
import com.amazonaws.mobileconnectors.dynamodbv2.document.datatype.Primitive;
import com.amazonaws.regions.Regions;
import com.amazonaws.services.dynamodbv2.AmazonDynamoDBClient;
import com.amazonaws.services.dynamodbv2.model.AttributeValue;
import com.amazonaws.services.dynamodbv2.model.QueryRequest;
import com.amazonaws.services.dynamodbv2.model.QueryResult;
import com.amazonaws.services.dynamodbv2.model.ReturnValue;
import com.amazonaws.services.dynamodbv2.model.ScanRequest;
import com.amazonaws.services.dynamodbv2.model.ScanResult;
import com.amazonaws.services.dynamodbv2.model.UpdateItemRequest;

import java.util.HashMap;
import java.util.List;
import java.util.Map;


class DynamoDatabase {

    private CognitoCachingCredentialsProvider credentialsProvider;
    private AmazonDynamoDBClient dbClient;
    private Table dbTable;
    private final String COGNITO_POOL_ID = "us-east-1:cac34d35-7441-4a51-ae5d-f89172bfaebd";
    private final Regions COGNITO_REGION = Regions.US_EAST_1;
    private final String DYNAMODB_TABLE = "smart_workspace";
    private Context context;

    private int employeeID;

    DynamoDatabase(Context context){
        this.context = context;
        employeeID = 0;
    }

    public void setEmployeeID(int employeeID){
        this.employeeID = employeeID;
    }

    public int getEmployeeID(){
        return this.employeeID;
    }

    public void incrementEmployeeID(){
        employeeID = employeeID + 1;
    }

    public void connect(){
        // Create a new credentials provider
        credentialsProvider = new CognitoCachingCredentialsProvider(
                context, COGNITO_POOL_ID, COGNITO_REGION);

        // Create a connection to DynamoDB
        dbClient = new AmazonDynamoDBClient(credentialsProvider);

        dbTable = Table.loadTable(dbClient, DYNAMODB_TABLE);
    }

    public void update(Employee employee){

        Document memo = new Document();
        incrementEmployeeID();
        memo.put("Email", employee.getUserName() + "@infineon.com");
        memo.put("Employee_ID", employeeID);
        memo.put("Username", employee.getUserName());
        memo.put("OnLeave", false);
        memo.put("Password", employee.getPassword());
        memo.put("Phone", Double.parseDouble(employee.getPhone()));
        memo.put("RFID_Number", Integer.parseInt(employee.getRFID()));
        memo.put("Work_Hours", "9:00 - 18:00");
        memo.put("Fan", Integer.parseInt(employee.getFan()));
        memo.put("Light", Integer.parseInt(employee.getLight()));

        dbTable.putItem(memo);
    }

    public void modify(Employee employee) {

        HashMap<String, AttributeValue> nameMap = new HashMap<String, AttributeValue>();
        nameMap.put("Employee_ID", new AttributeValue().withN(String.valueOf(employeeID)));
        HashMap<String, AttributeValue> valueMap = new HashMap<String, AttributeValue>();
        valueMap.put(":light", new AttributeValue().withN(String.valueOf(employee.getLight())));
        valueMap.put(":fan", new AttributeValue().withN(String.valueOf(employee.getFan())));
        valueMap.put(":phone", new AttributeValue().withN(String.valueOf(employee.getPhone())));
        valueMap.put(":password", new AttributeValue().withS(employee.getPassword()));
        valueMap.put(":rfid", new AttributeValue().withN(String.valueOf(employee.getRFID())));
        valueMap.put(":name", new AttributeValue().withS(employee.getUserName()));

        UpdateItemRequest updateItemRequest = new UpdateItemRequest()
                .withTableName(DYNAMODB_TABLE)
                .withUpdateExpression("SET Light=:light, Fan=:fan, Username=:name, RFID_Number=:rfid, Phone=:phone, Password=:password")
                .withKey(nameMap)
                .withExpressionAttributeValues(valueMap);

        dbClient.updateItem(updateItemRequest);
    }

    public Map getEmployeePrefs(int empId){

        HashMap<String, String> nameMap = new HashMap<String, String>();
        nameMap.put("#empID", "Employee_ID");
        HashMap<String, AttributeValue> valueMap = new HashMap<String, AttributeValue>();
        valueMap.put(":empIDValue", new AttributeValue().withN(String.valueOf(empId)));

        QueryRequest queryRequest = new QueryRequest()
                .withTableName(DYNAMODB_TABLE)
                .withKeyConditionExpression("#empID = :empIDValue")
                .withExpressionAttributeNames(nameMap)
                .withExpressionAttributeValues(valueMap);

        QueryResult queryResult = dbClient.query(queryRequest);

        List<Map<String, AttributeValue>> attributeValue = queryResult.getItems();

        if(attributeValue.size() > 0){
            return attributeValue.get(0);
        }else{
            return null;
        }
    }

    public int getEmployeeIDFromDB(){

        ScanRequest scanRequest = new ScanRequest()
                .withTableName(DYNAMODB_TABLE);

        ScanResult scanResult = dbClient.scan(scanRequest);

        employeeID = scanResult.getCount();

        Log.d("Last Employee_ID : ", String.valueOf(employeeID));

        if(employeeID > 0){
            return employeeID;
        }else{
            return 0;
        }
    }

}
