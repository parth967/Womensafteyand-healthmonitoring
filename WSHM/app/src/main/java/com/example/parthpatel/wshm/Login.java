package com.example.parthpatel.wshm;

import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

public class Login extends Activity {
    EditText username,password;
    String Username,Password;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

    }
    public void loginaccount(View view){
        username=(EditText)findViewById(R.id.loginusername);
        password=(EditText)findViewById(R.id.loginpassword);
        Username=username.getText().toString();
        Password=password.getText().toString();
        if(Username.equals("parth") && Password.equals("12345")){
            Toast.makeText(this,"Login Sucessfully",Toast.LENGTH_LONG).show();
            Intent intent=new Intent(Login.this,Home.class);
            startActivity(intent);
        }else{
            Toast.makeText(this,"Username Or Password Wrong",Toast.LENGTH_LONG).show();
        }
    }

}
