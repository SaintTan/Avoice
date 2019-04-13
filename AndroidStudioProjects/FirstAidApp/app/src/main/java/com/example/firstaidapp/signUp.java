package com.example.firstaidapp;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class signUp extends AppCompatActivity {

    private EditText email_inp;
    private EditText user_name;
    private EditText med_Id;
    private EditText password_first;
    private EditText password_conf;

    private TextView conf_pass;

    private Button sign_upBtn;

    private TextView as;
    private TextView bs;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.sign_up);

        email_inp = (EditText) findViewById(R.id.email_inp);
        user_name = (EditText) findViewById(R.id.user_name);
        med_Id = (EditText) findViewById(R.id.med_Id);
        password_first = (EditText) findViewById(R.id.password_first);
        password_conf = (EditText) findViewById(R.id.password_conf);

        conf_pass = (TextView) findViewById(R.id.conf_pass);

        as=(TextView) findViewById(R.id.as);
        bs=(TextView) findViewById(R.id.bs);

        sign_upBtn = (Button) findViewById(R.id.conf_signup);

        conf_pass.setVisibility(View.GONE);

        sign_upBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                check_same_pass(password_first.getText().toString(),password_conf.getText().toString());
            }
        });
    }

    private void check_same_pass(String pass_first, String pass_conf){
        if(pass_first.equals(pass_conf)){
            Intent moveBack= new Intent(signUp.this,loginDemo.class);
            startActivity(moveBack);
        }
        else{
            conf_pass.setVisibility(View.VISIBLE);
            as.setText(pass_first);
            bs.setText(pass_conf);
        }
    }
}