package com.example.smartworkspace;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.amazonaws.services.dynamodbv2.model.AttributeValue;

import java.util.Map;

public class MainActivity extends AppCompatActivity {

    private EditText etEmployeeID;
    private EditText etPassword;
    private Button btnLogin;
    private TextView tvRegister;

    private SharedPreferences sharedPreferences;
    private SharedPreferences.Editor editor;

    public static DynamoDatabase dynamoDB;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        /* initialize the UI views */
        ui_init();

        sharedPreferences = getSharedPreferences("USER_PREFS", MODE_PRIVATE);
        editor = sharedPreferences.edit();

        if(sharedPreferences != null){
            if(sharedPreferences.contains("EmpID")){
                etEmployeeID.setText(sharedPreferences.getString("EmpID", ""));
            }
        }

        dynamoDB = new DynamoDatabase(this);
        new DatabaseConnection().execute();

        /* Go to register activity for new users */
        tvRegister.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(MainActivity.this, RegistrationActivity.class));
            }
        });

        btnLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                if(validateCredentials()){
                    new CheckEmployeeCredentials().execute(Integer.parseInt(etEmployeeID.getText().toString()));
                }
            }
        });
    }

    private void ui_init(){
        etEmployeeID = findViewById(R.id.etUserID);
        etPassword = findViewById(R.id.etPassword);
        btnLogin = findViewById(R.id.btnLogin);
        tvRegister = findViewById(R.id.tvRegister);
    }

    private boolean validateCredentials(){

        String empID = etEmployeeID.getText().toString();
        String password = etPassword.getText().toString();

        if(empID.equals("") && password.equals("")){
            Toast.makeText(this, "Please enter valid credentials!", Toast.LENGTH_SHORT).show();
            return false;
        }else{
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
        }
    }

    public class CheckEmployeeCredentials extends AsyncTask<Integer, Void, Void> {

        Map employeeDetails;

        protected Void doInBackground(Integer... empIDs) {

            employeeDetails = dynamoDB.getEmployeePrefs(empIDs[0]);

            return null;
        }

        @Override
        protected void onPostExecute(Void aVoid) {
            super.onPostExecute(aVoid);

            AttributeValue passwordAttribute = (AttributeValue) employeeDetails.get("Password");

            String userPassword = passwordAttribute.getS();

            if(etPassword.getText().toString().equals(userPassword)){
                Toast.makeText(MainActivity.this, "Login Successful", Toast.LENGTH_LONG).show();
                startActivity(new Intent(MainActivity.this, UserProfile.class));
            }else{
                Toast.makeText(MainActivity.this, "Incorrect Credentials!", Toast.LENGTH_LONG).show();
            }
        }
    }


}