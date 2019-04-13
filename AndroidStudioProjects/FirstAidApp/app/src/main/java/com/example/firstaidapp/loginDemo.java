package com.example.firstaidapp;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import org.w3c.dom.Text;

public class loginDemo extends AppCompatActivity {
    private EditText user_email;
    private EditText password_text;

    private TextView no_info;

    private Button loginBtn;
    private Button signUp_Btn;

    private int counter = 5;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.login_demo);

        user_email = (EditText) findViewById(R.id.user_email);
        password_text = (EditText) findViewById(R.id.password_text);
        no_info = (TextView) findViewById(R.id.no_info);
        loginBtn = (Button) findViewById(R.id.loginBtn);
        signUp_Btn = (Button) findViewById(R.id.signUpBtn);

        loginBtn.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                validate(user_email.getText().toString(),password_text.getText().toString());
            }
        });

        signUp_Btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent signUpPage= new Intent(loginDemo.this, signUp.class);
                startActivity(signUpPage);
            }
        });
    }

    private void validate(String useremail, String userPass){
        if ((useremail.equals("a"))&&(userPass.equals("a"))){
            Intent loginInt = new Intent(loginDemo.this, MainActivity.class);
            startActivity(loginInt);
        }
        else{
            counter--;
            no_info.setText("No of attempts remaining : " + String.valueOf(counter));
            if(counter<=0){
                loginBtn.setEnabled(false);
            }
        }
    }
}
