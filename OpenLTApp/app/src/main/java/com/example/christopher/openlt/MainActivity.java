package com.example.christopher.openlt;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import app.akexorcist.bluetotohspp.library.BluetoothSPP;
import app.akexorcist.bluetotohspp.library.BluetoothState;
import app.akexorcist.bluetotohspp.library.DeviceList;

public class MainActivity extends AppCompatActivity {

    final String ON = "1";
    final String OFF = "0";

    BluetoothSPP bluetooth;

    Button connect;
    Button on;
	Button Server;
	Button Clientbtn;

	Server server;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);



        bluetooth = new BluetoothSPP(this);

        connect = (Button) findViewById(R.id.connect);
        on = (Button) findViewById(R.id.on);
		Server = (Button) findViewById(R.id.ServerButton);
		Clientbtn = (Button) findViewById(R.id.ClientButton);

        if (!bluetooth.isBluetoothAvailable()) {
            Toast.makeText(getApplicationContext(), "Bluetooth is not available", Toast.LENGTH_SHORT).show();
            //finish();
        }

        bluetooth.setBluetoothConnectionListener(new BluetoothSPP.BluetoothConnectionListener() {
            public void onDeviceConnected(String name, String address) {
                connect.setText("Connected to " + name);
            }

            public void onDeviceDisconnected() {
                connect.setText("Connection lost");
            }

            public void onDeviceConnectionFailed() {
                connect.setText("Unable to connect");
            }
        });

        connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (bluetooth.getServiceState() == BluetoothState.STATE_CONNECTED) {
                    bluetooth.disconnect();
                } else {
                    Intent intent = new Intent(getApplicationContext(), DeviceList.class);
                    startActivityForResult(intent, BluetoothState.REQUEST_CONNECT_DEVICE);
                }
            }
        });

        on.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                bluetooth.send(ON, true);
            }
        });

        Server.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    //System.out.println(InternetHelper.getIPAddress(false));
					server = new Server((MainActivity) getParent());
					System.out.println(server.getIpAddress()+":"+server.getPort());

				} catch (Exception e) {
                    e.printStackTrace();
                }
//                    String text = "";
//                    for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements(); ) {
//                        NetworkInterface intf = en.nextElement();
//                        for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements(); ) {
//                            InetAddress inetAddress = enumIpAddr.nextElement();
//                            if (!inetAddress.isLoopbackAddress() ) {
//
//                                text = inetAddress.getHostAddress();
//                            }
//                        }
//                    }
//                    System.out.println(text);
//
//                    URL url = new URL("http://www.ip-api.com/json");
//                    InputStream is = url.openStream();
//                    BufferedReader br = new BufferedReader(new InputStreamReader(is));
//                    String text = "";
//                    String line;
//                    while ( (line = br.readLine()) != null)
//                        text += line;
//
//                    br.close();
//                    is.close();
//                    Toast.makeText(getApplicationContext()
//                            , text
//                            , Toast.LENGTH_SHORT).show();
//                } catch (Exception e) {
//                    e.printStackTrace();
//                }

            }
        });


		Clientbtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {

					Client myClient = new Client("192.168.200.2"
							, 8080);
					myClient.execute();
				}
		});
	}



    public void onStart() {
        super.onStart();
        if (!bluetooth.isBluetoothEnabled()) {
            bluetooth.enable();
        } else {
            if (!bluetooth.isServiceAvailable()) {
                bluetooth.setupService();
                bluetooth.startService(BluetoothState.DEVICE_OTHER);
            }
        }
    }


    public void onDestroy() {
        super.onDestroy();
        bluetooth.stopService();
    }

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == BluetoothState.REQUEST_CONNECT_DEVICE) {
            if (resultCode == Activity.RESULT_OK)
                bluetooth.connect(data);
        } else if (requestCode == BluetoothState.REQUEST_ENABLE_BT) {
            if (resultCode == Activity.RESULT_OK) {
                bluetooth.setupService();
            } else {
                Toast.makeText(getApplicationContext()
                        , "Bluetooth was not enabled."
                        , Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }

}