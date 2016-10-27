package com.example.razvan.wsnmonitor;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.TaskStackBuilder;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v4.app.NotificationCompat;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends Activity {


    String str="";
    ArrayList<String> names ;
    ArrayList<String> values;
    ArrayList<String> dates;
    ArrayList<String> extras;

    ArrayList<String> hist_temp;
    ArrayList<String> hist_humid;
    ArrayList<String> hist_pressure;
    ArrayList<String> hist_motion;
    ArrayList<String> hist_gas;

    Button btn_temp,btn_humid,btn_pressure;
    TextView tv_temp,tv_humid,tv_pressure,tv_motion,tv_gas;

    int idx_temp = 0,idx_pressure = 1, idx_motion=2, idx_humidity=3, idx_gas=4;
    int notificationNumber=0;

    Intent i;
    Intent currentIntent;
    DownloadFileFromURL dwnld;

    NotificationCompat.Builder mBuilder;
    NotificationManager mNotificationManager;

    Context context;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        names = new ArrayList<>();
        values = new ArrayList<>();
        dates = new ArrayList<>();
        extras = new ArrayList<>();

        hist_temp = new ArrayList<>();
        hist_humid = new ArrayList<>();
        hist_pressure = new ArrayList<>();
        hist_motion = new ArrayList<>();
        hist_gas = new ArrayList<>();

        mBuilder = new NotificationCompat.Builder(this);

        i = new Intent(this,Details.class);
        currentIntent = new Intent(this,MainActivity.class);
        context = getApplicationContext();

        mNotificationManager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        mNotificationManager.cancel(1);

        registerUIelements();
        registerCallbacks();

        new DownloadFileFromURL().execute();

        int refresh_rate = 4000;
        new Timer().scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                new DownloadFileFromURL().execute();
            }
        },0,refresh_rate);


    }

    private void registerCallbacks() {




        btn_temp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                i.putStringArrayListExtra("history",hist_temp);
                i.putExtra("name", "Temperature: ");
                i.putExtra("current",tv_temp.getText());
                startActivity(i);
            }
        });


        btn_humid.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                i.putStringArrayListExtra("history",hist_humid);
                i.putExtra("name", "Humidity: ");
                i.putExtra("current",tv_humid.getText());
                startActivity(i);
            }
        });


        btn_pressure.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                i.putStringArrayListExtra("history",hist_pressure);
                i.putExtra("name", "Pressure: ");
                i.putExtra("current",tv_pressure.getText());
                startActivity(i);
            }
        });




    }

    private void registerUIelements() {


        tv_gas = (TextView)findViewById(R.id.tv_gas);

        btn_temp = (Button)findViewById(R.id.btn_temp);
        tv_temp = (TextView)findViewById(R.id.tv_temp);

        btn_humid = (Button)findViewById(R.id.btn_humid);
        tv_humid = (TextView)findViewById(R.id.tv_humid);

        btn_pressure = (Button)findViewById(R.id.btn_pressure);
        tv_pressure = (TextView)findViewById(R.id.tv_pressure);


        tv_motion = (TextView)findViewById(R.id.tv_motion);

    }


    class DownloadFileFromURL extends AsyncTask<String, String, String> {


        /**
         * Before starting background thread Show Progress Bar Dialog
         */
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
        }

        @Override
        protected void onPostExecute(String s) {
            super.onPostExecute(s);


            String lines[] = str.split("\n");

            for (String l:lines) {
                String tmp[] = l.split("\t");
                names.add(tmp[0]);
                values.add(tmp[1]);
                extras.add(tmp[2]);
                dates.add(tmp[3]);
            }

            tv_temp.setText("Temperature: "+values.get(idx_temp) + " \u2103 ");
            tv_humid.setText("Humidity: "+values.get(idx_humidity) + "% ");
            tv_pressure.setText("Pressure: "+values.get(idx_pressure) + "mmHg");
            tv_motion.setText(""+extras.get(idx_motion));
            tv_gas.setText("Gas:" + extras.get(idx_gas));

            hist_temp.add(values.get(idx_temp));
            hist_humid.add(values.get(idx_humidity));
            hist_pressure.add(values.get(idx_pressure));


            if((extras.get(idx_gas).toLowerCase().equals("danger")) && notificationNumber == 0){
                /* send notification */
                notificationNumber++;
                mBuilder.setContentTitle("Gas warning!");
                mBuilder.setContentText("High gas level!");
                mBuilder.setSmallIcon(R.mipmap.ic_launcher);
                mBuilder.setNumber(notificationNumber);

                currentIntent.putExtra("notificationId", notificationNumber);

                TaskStackBuilder stackBuilder = TaskStackBuilder.create(context);
                stackBuilder.addNextIntent(currentIntent);
                PendingIntent pendingIntent = stackBuilder.getPendingIntent(0, PendingIntent.FLAG_ONE_SHOT);
                mBuilder.setContentIntent(pendingIntent);
                Notification notification = mBuilder.build();
                notification.defaults |= Notification.DEFAULT_VIBRATE | Notification.DEFAULT_SOUND;
                mNotificationManager = (NotificationManager)getSystemService(Context.NOTIFICATION_SERVICE);

                mNotificationManager.notify(notificationNumber,notification);

            }
            else{
                if(!extras.get(idx_gas).toLowerCase().equals("danger")){
                    notificationNumber=0;
                }
            }


        }

        /**
         * Downloading file in background thread
         * */
        @Override
        protected String doInBackground(String... params) {


            names = new ArrayList<>();
            values = new ArrayList<>();
            dates = new ArrayList<>();
            extras = new ArrayList<>();

            str = "";

            try {
                URL url = new URL("http://192.168.43.164/latest.txt");
                URLConnection con = url.openConnection();
                con.setUseCaches(false);
                con.connect();
                BufferedReader in = new BufferedReader(new InputStreamReader(con.getInputStream()));

                String line;

                while((line=in.readLine())!=null){
                    str+=line+"\n";
                }

            }
            catch(Exception e){
                Log.d("ImageManager", "Error: " + e);
            }
            return null;
        }

    }


}
