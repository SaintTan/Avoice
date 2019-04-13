package com.example.firstaidapp;

import android.app.Notification;
import android.app.NotificationManager;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationManagerCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {

    private NotificationManagerCompat notificationManager;
    private EditText FirstInput;
    private EditText SecondInput;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        notificationManager= NotificationManagerCompat.from(this);
   //     FirstInput = findViewById(R.id.firstInput);
   //     SecondInput = findViewById(R.id.second_Input);


    }

    public void sendChannel1 (View v){
        String firstInput = FirstInput.getText().toString();
        String secondInput = SecondInput.getText().toString();

        Notification notification = new NotificationCompat.Builder(this,notiClass.Channel_1_ID)
                .setSmallIcon(R.drawable.ic_accessibility_black_24dp)
                .setContentTitle(firstInput)
                .setContentText(secondInput)
                .build();

        notificationManager.notify(1,notification);
    }

    public void sendChannel2 (View v){
        String firstInput = FirstInput.getText().toString();
        String secondInput = SecondInput.getText().toString();

        Notification notification = new NotificationCompat.Builder(this,notiClass.Channel_2_ID)
                .setSmallIcon(R.drawable.ic_accessibility_black_24dp)
                .setContentTitle(firstInput)
                .setContentText(secondInput)
                .build();

        notificationManager.notify(2,notification);
    }

}
