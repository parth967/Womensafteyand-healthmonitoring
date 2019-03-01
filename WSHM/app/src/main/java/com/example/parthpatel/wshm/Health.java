package com.example.parthpatel.wshm;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;
import android.os.Handler;
import java.util.logging.LogRecord;

public class Health extends AppCompatActivity{
    int intVal=1;
    BluetoothAdapter bluetoothAdapter=null;
    BluetoothSocket bluetoothSocket=null;
    TextView temptext;
    String address=null,name=null;
    Set<BluetoothDevice> btdevice=null;
    InputStream inputStream;
    Thread workerThread;
    byte[] readBuffer;
    int readBufferPosition;
    int counter;
    volatile boolean stopWorker;
    static final UUID uuid=UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_health);
        temptext=(TextView)findViewById(R.id.temptext);
       // temptext.setText("Temperature");
        bluetoothAdapter= BluetoothAdapter.getDefaultAdapter();
        if(bluetoothAdapter==null){
            Toast.makeText(getApplicationContext(),"Bluetoth dose not support",Toast.LENGTH_SHORT).show();
        }else {
            if(!bluetoothAdapter.isEnabled()){
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, intVal);

            }else{
                Toast.makeText(getApplicationContext(),"Bluetooth is already Enable",Toast.LENGTH_SHORT).show();
            }
        }
    }
    public void connect(View view) throws IOException {

        bluetooth_connect_device();

    }
    public void temp(View view) throws IOException {
        tempsend();
    }
    public void heart(View view){
        heartsend();
    }
    private void bluetooth_connect_device() throws IOException
    {
        try
        {
           bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
            address = bluetoothAdapter.getAddress();
            btdevice= bluetoothAdapter.getBondedDevices();
            if (btdevice.size()>0)
            {
                for(BluetoothDevice bt : btdevice)
                {
                    address=bt.getAddress().toString();name = bt.getName().toString();
                    Toast.makeText(getApplicationContext(),"Connected", Toast.LENGTH_SHORT).show();

                }
            }

        }
        catch(Exception we){}
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
        BluetoothDevice dispositivo =bluetoothAdapter.getRemoteDevice(address);//connects to the device's address and checks if it's available
        bluetoothSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(uuid);//create a RFCOMM (SPP) connection
        bluetoothSocket.connect();

    }

    private void tempsend()
    {String i="a";
        try
        {
            if (bluetoothSocket!=null)
            {

                bluetoothSocket.getOutputStream().write(i.toString().getBytes());
               inputStream=bluetoothSocket.getInputStream();
               beginlistenfordata();
                // int txt=bluetoothSocket.getInputStream().read();
               // String temperature=new String(String.valueOf(txt).getBytes(),"UTF-8");
                //temptext.setText(temperature);


            }

        }
        catch (Exception e)
        {
            Toast.makeText(getApplicationContext(),"Exception: "+e.getMessage(), Toast.LENGTH_SHORT).show();

        }

    }
    public void beginlistenfordata(){
        final Handler handler = new Handler();
        final byte delimiter = 10; //This is the ASCII code for a newline character

        stopWorker = false;
        readBufferPosition = 0;
        readBuffer = new byte[1024000];
        workerThread = new Thread(new Runnable()
        {
            public void run()
            {
                while(!Thread.currentThread().isInterrupted() && !stopWorker)
                {
                    try
                    {
                        int bytesAvailable = inputStream.available();
                        if(bytesAvailable > 0)
                        {
                            byte[] packetBytes = new byte[bytesAvailable];
                            inputStream.read(packetBytes);
                            for(int i=0;i<bytesAvailable;i++)
                            {
                                byte b = packetBytes[i];
                                if(b == delimiter)
                                {
                                    byte[] encodedBytes = new byte[readBufferPosition];
                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                    final String data = new String(encodedBytes, "US-ASCII");
                                    readBufferPosition = 0;

                                    handler.post(new Runnable()
                                    {
                                        public void run()
                                        {
                                            temptext.setText(data);
                                        }
                                    });
                                }
                                else
                                {
                                    readBuffer[readBufferPosition++] = b;
                                }
                            }
                        }
                    }
                    catch (IOException ex)
                    {
                        stopWorker = true;
                    }
                }
            }
        });

        workerThread.start();

    }
    private void heartsend()
    {String i="b";
        try
        {
            if (bluetoothSocket!=null)
            {

                bluetoothSocket.getOutputStream().write(i.toString().getBytes());
            }

        }
        catch (Exception e)
        {
            Toast.makeText(getApplicationContext(),e.getMessage(), Toast.LENGTH_SHORT).show();

        }

    }


}
