package com.example.hangnhold;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;


import android.Manifest;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;

import android.bluetooth.BluetoothSocket;
import android.content.Intent;

import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;

import android.widget.ArrayAdapter;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    private static final int REQUEST_ENABLE_BT = 1;

    private static final UUID MY_UUID = UUID.fromString("fa87c0d0-afac-11de-8a39-0800200c9a66");

    private LinearLayout btn_module1, btn_module2, btn_measure, btn_calibrate;
    private TextView btn_module1_text, btn_module2_text, btn_measure_text, btn_calibrate_text;
    private BluetoothAdapter bluetoothAdapter;

    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        checkBtPermissions();
        init();
        //filters();
        onClicks();
        enableBt();
    }

    /*
    public void filters() {
        IntentFilter filter = new IntentFilter();
        filter.addAction(BluetoothDevice.ACTION_ACL_CONNECTED);
        filter.addAction(BluetoothDevice.ACTION_ACL_DISCONNECT_REQUESTED);
        filter.addAction(BluetoothDevice.ACTION_ACL_DISCONNECTED);
        this.registerReceiver(broadcastReceiver, filter);
    }
     */

    private void init() {

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        btn_module1 = findViewById(R.id.btn_module1);
        btn_module2 = findViewById(R.id.btn_module2);
        btn_measure = findViewById(R.id.btn_measure);
        btn_calibrate = findViewById(R.id.btn_calibrate);


        /*
        btn_module1_text = findViewById(R.id.btn_module1_text);
        btn_module2_text = findViewById(R.id.btn_module2_text);
        btn_measure_text = findViewById(R.id.btn_measure_text);
        btn_calibrate_text = findViewById(R.id.btn_calibrate_text);

        tv_name = (TextView) findViewById(R.id.tv_name);
        tv_mac = (TextView) findViewById(R.id.tv_mac);
        */
        log("Everything initialized");
    }

    public void onClicks() {
        btn_module1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(bluetoothAdapter.isEnabled()) {
                    toast("Pressed btn_module1");
                    Intent intent = new Intent(MainActivity.this, ListDevices.class);
                    startActivity(intent);
                } else {
                    enableBt();
                }
            }
        });

        btn_module2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            }
        });

        btn_measure.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            }
        });

        btn_calibrate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            }
        });
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @RequiresApi(api = Build.VERSION_CODES.M)
    public void checkBtPermissions() {
        if(Build.VERSION.SDK_INT > Build.VERSION_CODES.LOLLIPOP){
            int permissionCheck = this.checkSelfPermission("Manifest.permission.ACCESS_FINE_LOCATION");
            permissionCheck += this.checkSelfPermission("Manifest.permission.ACCESS_COARSE_LOCATION");

            log("Permissions are checked");
            if (permissionCheck != 0) {

                this.requestPermissions(new String[]{Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION}, 1001); //Any number
            }
        } else{
            log("checkBTPermissions: No need to check permissions. SDK version < LOLLIPOP.");
        }
    }


    public void enableBt() {
        if(bluetoothAdapter == null || !bluetoothAdapter.isEnabled()) {
            Intent enableBTIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBTIntent, REQUEST_ENABLE_BT);

            log("Enabled Bluetooth");
        }
    }

    private void toast(String string) {
        Toast.makeText(getApplicationContext(), string, Toast.LENGTH_SHORT).show();
    }

    private void log(String string){
        Log.d(TAG, string);
    }
}