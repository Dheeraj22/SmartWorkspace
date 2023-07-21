package com.example.smartworkspace;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.amazonaws.services.dynamodbv2.model.AttributeValue;

import java.util.Map;

public class RegistrationActivity extends AppCompatActivity {

    private TextView tvRegEmpID;
    private EditText etRegName;
    private EditText etRegPhone;
    private EditText etRegPassword;
    private EditText etRegRFID;
    private EditText etRegLight;
    private EditText etRegFan;
    private Button btnRegister;

    public static Employee employee;
    DynamoDatabase dynamoDB;

    private SharedPreferences sharedPreferences;
    private SharedPreferences.Editor editor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_registration);

        getSupportActionBar().setTitle("Employee Registration");
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        /* initialize the UI views */
        ui_init();

        sharedPreferences = getSharedPreferences("USER_PREFS", MODE_PRIVATE);
        editor = sharedPreferences.edit();

        /* Connect to Database */
        dynamoDB = new DynamoDatabase(this);
        new DatabaseConnection().execute();

        /* Go to login  */
        btnRegister.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(validateCredentials()){
                    startActivity(new Intent(RegistrationActivity.this, MainActivity.class));
                }
            }
        });
    }

    private void ui_init(){
        etRegName = findViewById(R.id.etProfileName);
        etRegPhone = findViewById(R.id.etProfilePhone);
        etRegPassword = findViewById(R.id.etProfilePassword);
        etRegRFID = findViewById(R.id.etProfileRFID);
        etRegLight = findViewById(R.id.etProfileLight);
        etRegFan = findViewById(R.id.etProfileFan);
        btnRegister = findViewById(R.id.btnUpdate);
        tvRegEmpID = findViewById(R.id.tvProfileID);
    }

    private boolean validateCredentials(){

        String empID = tvRegEmpID.getText().toString();
        String name = etRegName.getText().toString();
        String phone = etRegPhone.getText().toString();
        String password = etRegPassword.getText().toString();
        String rfid = etRegRFID.getText().toString();
        String light = etRegLight.getText().toString();
        String fan = etRegFan.getText().toString();

        if(empID.equals("") && password.equals("") && name.equals("") && phone.equals("") && rfid.equals("")
                                                && light.equals("") && fan.equals(""))
        {
            Toast.makeText(this, "Please enter all the details!", Toast.LENGTH_SHORT).show();
            return false;
        }else{

            employee = new Employee(name, empID, rfid, password, phone, light, fan);

            /* Add employee to database */
            new UpdateDatabase().execute();

            // TODO
            return true;
        }
    }

    public class DatabaseConnection extends AsyncTask<Void, Void, Void> {

        protected Void doInBackground(Void... Void) {

            /* Connect to DynamoDB */
            dynamoDB.connect();

            return null;
        }

        @Override
        protected void onPostExecute(Void aVoid) {
            super.onPostExecute(aVoid);

            /* Fetch last added employee ID */
            new getEmployeeID().execute();
        }
    }

    public class UpdateDatabase extends AsyncTask<Void, Void, Boolean> {

        protected Boolean doInBackground(Void... Void) {

            /* Add new employee record */
            try {
                dynamoDB.update(employee);
            }catch (Exception e){
                Log.d("Query Error: ", e.toString());
                return false;
            }

            return true;
        }

        @Override
        protected void onPostExecute(Boolean result) {
            super.onPostExecute(result);

            if(result){
                Toast.makeText(getApplicationContext(), "Registration Successful!", Toast.LENGTH_LONG).show();
                editor.putString("EmpID", String.valueOf(dynamoDB.getEmployeeID()));
                editor.apply();
            }else{
                Toast.makeText(getApplicationContext(), "Registration Failed! Contact IT!", Toast.LENGTH_LONG).show();
            }
        }
    }

    public class getEmployeeID extends AsyncTask<Void, Void, Integer> {

        int employeeID = 0;

        protected Integer doInBackground(Void... Void) {

            /* Add new employee record */
            try {
                employeeID = dynamoDB.getEmployeeIDFromDB();
            }catch (Exception e){
                Log.d("ERROR: ", e.toString());
                return 0;
            }

            return employeeID;
        }

        @Override
        protected void onPostExecute(Integer result) {
            super.onPostExecute(result);

            if(employeeID >= 0){
                tvRegEmpID.setText("Your Employee ID is: " + String.valueOf(employeeID + 1));
                dynamoDB.setEmployeeID(employeeID);

            }else{
                Toast.makeText(getApplicationContext(), "Not able to fetch Employee ID!", Toast.LENGTH_LONG).show();
            }
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                startActivity(new Intent(RegistrationActivity.this, MainActivity.class));
                break;
        }
        return true;
    }
}