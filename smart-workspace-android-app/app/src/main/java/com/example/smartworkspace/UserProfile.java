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

public class UserProfile extends AppCompatActivity {

    private TextView tvProfileID;
    private EditText etProfileName;
    private EditText etProfilePhone;
    private EditText etProfilePassword;
    private EditText etProfileRFID;
    private EditText etProfileLight;
    private EditText etProfileFan;
    private Button btnSave;

    private Employee employee;

    private SharedPreferences sharedPreferences;
    private SharedPreferences.Editor editor;

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        startActivity(new Intent(UserProfile.this, MainActivity.class));
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_profile);

        getSupportActionBar().setTitle("User Preferences");
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        /* initialize the UI views */
        ui_init();

        sharedPreferences = getSharedPreferences("USER_PREFS", MODE_PRIVATE);
        editor = sharedPreferences.edit();

        if(sharedPreferences != null){
            if(sharedPreferences.contains("EmpID")){
                tvProfileID.setText(sharedPreferences.getString("EmpID", ""));
            }
        }

        new QueryDatabase().execute();

        btnSave.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                if(validateCredentials()){
                    /* Update employee prefs in database */
                    new UpdateDatabase().execute(employee);
                }
            }
        });
    }

    private void ui_init(){
        tvProfileID = findViewById(R.id.tvProfileID);
        etProfileName = findViewById(R.id.etProfileName);
        etProfilePhone = findViewById(R.id.etProfilePhone);
        etProfilePassword = findViewById(R.id.etProfilePassword);
        etProfileRFID = findViewById(R.id.etProfileRFID);
        etProfileLight = findViewById(R.id.etProfileLight);
        etProfileFan = findViewById(R.id.etProfileFan);
        btnSave = findViewById(R.id.btnUpdate);
    }

    private boolean validateCredentials(){

        String empID = tvProfileID.getText().toString();
        String name = etProfileName.getText().toString();
        String phone = etProfilePhone.getText().toString();
        String password = etProfilePassword.getText().toString();
        String rfid = etProfileRFID.getText().toString();
        String light = etProfileLight.getText().toString();
        String fan = etProfileFan.getText().toString();

        if(empID.equals("") && password.equals("") && name.equals("") && phone.equals("") && rfid.equals("")
                && light.equals("") && fan.equals(""))
        {
            Toast.makeText(this, "Details can't be left empty!", Toast.LENGTH_SHORT).show();
            return false;
        }else{
            employee = new Employee(name, empID, rfid, password, phone, light, fan);
            return true;
        }
    }

    public class QueryDatabase extends AsyncTask<Void, Void, Boolean> {

        Map record;

        protected Boolean doInBackground(Void... Void) {

            /* Get employee record */
            try {
                record = MainActivity.dynamoDB.getEmployeePrefs(Integer.parseInt(sharedPreferences.getString("EmpID", "")));
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

                Toast.makeText(getApplicationContext(), "Fetched details successfully!", Toast.LENGTH_LONG).show();

                Log.d("Map: ", record.toString());

                AttributeValue id_attr = (AttributeValue) record.get("Employee_ID");
                AttributeValue phone_attr = (AttributeValue) record.get("Phone");
                AttributeValue name_attr = (AttributeValue) record.get("Username");
                AttributeValue light_attr = (AttributeValue) record.get("Light");
                AttributeValue fan_attr = (AttributeValue) record.get("Fan");
                AttributeValue rfid_attr = (AttributeValue) record.get("RFID_Number");
                AttributeValue password_attr = (AttributeValue) record.get("Password");

                String empID = id_attr.getN();
                String name = name_attr.getS();
                String phone = phone_attr.getN();
                String light = light_attr.getN();
                String fan = fan_attr.getN();
                String rfid = rfid_attr.getN();
                String password = password_attr.getS();

                MainActivity.dynamoDB.setEmployeeID(Integer.parseInt(empID));
                tvProfileID.setText("Your Employee ID is: " + empID);
                etProfileName.setText(name);
                etProfilePassword.setText(password);
                etProfileFan.setText(fan);
                etProfileRFID.setText(rfid);
                etProfileLight.setText(light);
                etProfilePhone.setText(phone);

            }else{
                Toast.makeText(getApplicationContext(), "Unable to fetch details! Contact IT!", Toast.LENGTH_LONG).show();
            }
        }
    }

    public class UpdateDatabase extends AsyncTask<Employee, Void, Boolean> {

        protected Boolean doInBackground(Employee... employees) {

            /* Update employee record */
            try {
                MainActivity.dynamoDB.modify(employees[0]);
            }catch (Exception e){
                Log.d("Update Error: ", e.toString());
                return false;
            }

            return true;
        }

        @Override
        protected void onPostExecute(Boolean result) {
            super.onPostExecute(result);

            if(result){
                Toast.makeText(getApplicationContext(), "Updated details successfully!", Toast.LENGTH_LONG).show();
            }else{
                Toast.makeText(getApplicationContext(), "Unable to update details! Contact IT!", Toast.LENGTH_LONG).show();
            }
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                startActivity(new Intent(UserProfile.this, MainActivity.class));
                break;
        }
        return true;
    }
}