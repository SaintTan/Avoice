package com.example.firstaidapp;

import android.app.Application;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.os.Build;

public class notiClass extends Application {

    public static final String Channel_1_ID = "channel1";
    public static final String Channel_2_ID = "channel2";

    @Override
    public void onCreate() {
        super.onCreate();
        createNotificatoinChannels();
    }

    private void createNotificatoinChannels(){
        if (Build.VERSION.SDK_INT>=Build.VERSION_CODES.O){
            NotificationChannel channel1= new NotificationChannel(
                    Channel_1_ID,
                    "Help Needed! AiDent",
                    NotificationManager.IMPORTANCE_DEFAULT
                    );
                    channel1.setDescription("%s is requesting for help. Tap for more info.");

            NotificationChannel channel2= new NotificationChannel(
                    Channel_2_ID,
                    "Emergency! AiDent",
                    NotificationManager.IMPORTANCE_HIGH
            );
            channel2.setDescription("%s Needs emergency help! Tap for more info.");

            NotificationManager manager = getSystemService(NotificationManager.class);
            manager.createNotificationChannel(channel1);
            manager.createNotificationChannel(channel2);
        }

    }
}
