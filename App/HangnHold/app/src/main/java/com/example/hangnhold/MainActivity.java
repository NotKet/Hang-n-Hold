package com.example.hangnhold;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;


import android.Manifest;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Set;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    private static final int REQUEST_ENABLE_BT = 1;

    private BluetoothAdapter bluetoothAdapter = null;
    private Button btn_module1, btn_module2, btn_measurement, btn_calibration;
    private TextView tv_name, tv_mac;
    private ListView lvNewDevices;
    private ArrayList arrayList;
    private ArrayAdapter arrayAdapter;



    private void init() {
        btn_module1 = (Button) findViewById(R.id.btn_module1);
        btn_module2 = (Button) findViewById(R.id.btn_module2);
        btn_measurement = (Button) findViewById(R.id.btn_measurement);
        btn_calibration = (Button) findViewById(R.id.btn_calibration);

        tv_name = (TextView) findViewById(R.id.tv_name);
        tv_mac = (TextView) findViewById(R.id.tv_mac);

        lvNewDevices = (ListView) findViewById(R.id.lvNewDevices);

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        arrayList = new ArrayList();
        arrayAdapter= new ArrayAdapter<String>(getApplicationContext(), android.R.layout.simple_list_item_1, arrayList);
        lvNewDevices.setAdapter(arrayAdapter);

        log("Everything initialized");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        init();
        enableBt();

        btn_module1.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.M)
            public void onClick(View v) {
                //listPairedDevices();
                discoverDevices();
            }
        });
    }

    @RequiresApi(api = Build.VERSION_CODES.M)
    private void discoverDevices() {
        if(bluetoothAdapter.isEnabled()) {
            if(!bluetoothAdapter.isDiscovering()) {
                bluetoothAdapter.startDiscovery();

                IntentFilter filter = new IntentFilter();

                filter.addAction(BluetoothDevice.ACTION_FOUND);
                filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
                filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);

                registerReceiver(broadcastReceiver, filter);
            }
        } else {
            toast("Turn on Bluetooth");
        }
    }

    private final BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            if (BluetoothAdapter.ACTION_DISCOVERY_STARTED.equals(action)) {
                log("Discovery started");
            } else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {
                log("Discovery finished");
            } else if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice device = (BluetoothDevice) intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);

                arrayList.add(device.getName() + "\n" + device.getAddress());
                arrayAdapter.notifyDataSetChanged();

                toast("Found device " + device.getName());
            }
        }
    };

    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(broadcastReceiver);
    }

    public void listPairedDevices() {
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            for(BluetoothDevice device : pairedDevices) {
                String deviceName = device.getName();
                String macAddress = device.getAddress();

                tv_name.append(deviceName + "\n");
                tv_mac.append(macAddress + "\n");
            }
        }
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