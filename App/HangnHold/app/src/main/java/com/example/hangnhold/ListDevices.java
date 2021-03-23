package com.example.hangnhold;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;

public class ListDevices extends Activity {
    private static final String TAG = "ListDevice";

    private ImageView btn_back;
    private ListView lvNewDevices;
    private ArrayList<String> deviceList = new ArrayList<String>();
    private BluetoothAdapter bluetoothAdapter;

    private BluetoothSocket bluetoothSocket;

    private final static int REQUEST_ENABLE_BT = 1; // used to identify adding bluetooth names
    private final static int MESSAGE_READ = 2; // used in bluetooth handler to identify message update
    private final static int CONNECTING_STATUS = 3; // used in bluetooth handler to identify message status

    private static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    @Override
    protected void onCreate(Bundle savedInstanceSate) {
        super.onCreate(savedInstanceSate);
        setContentView(R.layout.list_devices);

        init();
        onClicks();
        listPairedDevices();
    }

    public void init() {
        btn_back = findViewById(R.id.btn_back);
        lvNewDevices = findViewById(R.id.lvNewDevices);
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        //lvNewDevices.setOnItemClickListener(deviceClickListener);
    }

    public void listPairedDevices() {
        log("Finding Paired devices");
        bluetoothAdapter.startDiscovery();

        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        registerReceiver(mReceiver, filter);
        /*
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        if(bluetoothAdapter.isEnabled()) {
            if (pairedDevices.size() > 0) {
                for(BluetoothDevice device : pairedDevices) {
                    btArrayAdapter.add( device.getName() + "\n" + device.getAddress());
                    log(device.getName() + ": " + device.getAddress());
                }
            }
        } else {
            toast("Bluetooth is disabled");
        }

         */
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(mReceiver);
        bluetoothAdapter.cancelDiscovery();
    }

    @Override
    protected void onStop() {
        super.onStop();
        bluetoothAdapter.cancelDiscovery();
    }

    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                deviceList.add(device.getName() + "\n" + device.getAddress());

                log(device.getName() + "\n" + device.getAddress());
                lvNewDevices.setAdapter(new ArrayAdapter<String>(context, android.R.layout.simple_list_item_1, deviceList));
            }
        }
    };

    public void onClicks() {
        btn_back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ListDevices.this, MainActivity.class);
                startActivity(intent);
            }
        });

        lvNewDevices.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                bluetoothAdapter.cancelDiscovery();

                final String info = ((TextView) view).getText().toString();

                String address = info.substring(info.length()-17);


                BluetoothDevice connect_device = bluetoothAdapter.getRemoteDevice(address);

                try {
                    bluetoothSocket = connect_device.createRfcommSocketToServiceRecord(myUUID);
                    bluetoothSocket.connect();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    private void toast(String string) {
        Toast.makeText(getApplicationContext(), string, Toast.LENGTH_SHORT).show();
    }

    private void log(String string){
        Log.d(TAG, string);
    }
}


