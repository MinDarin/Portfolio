package com.example.light_test;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.SeekBar;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedWriter;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;

public class SubActivity extends AppCompatActivity {

    TCPclient tcpThread = new TCPclient();
    Handler mHandler = tcpThread.getmHandler();
    int rate = 0;
        @Override
        protected void onCreate(@Nullable Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            setContentView(R.layout.light_activity_main);

            SeekBar seekBar1y = findViewById(R.id.light1_yellow_rate);
            seekBar1y.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                @Override
                public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) { }
                @Override
                public void onStartTrackingTouch(SeekBar seekBar) { }
                @Override
                public void onStopTrackingTouch(SeekBar seekBar) {
                    int rate;
                    rate = seekBar.getProgress();
                    Message message = new Message();
                    message.what = 1;
                    message.arg1 = 1;
                    message.arg2 = rate;
                    mHandler.sendMessage(message);
                }
            });

            SeekBar seekBar1w = findViewById(R.id.light1_white_rate);
            seekBar1w.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                @Override
                public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) { }
                @Override
                public void onStartTrackingTouch(SeekBar seekBar) { }
                @Override
                public void onStopTrackingTouch(SeekBar seekBar) {
                    rate = seekBar.getProgress();
                    Message message = new Message();
                    message.what = 1;
                    message.arg1 = 0;
                    message.arg2 = rate;
                    mHandler.sendMessage(message);
                }
            });

            SeekBar seekBar2y = findViewById(R.id.light2_yellow_rate);
            seekBar2y.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                @Override
                public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) { }
                @Override
                public void onStartTrackingTouch(SeekBar seekBar) { }
                @Override
                public void onStopTrackingTouch(SeekBar seekBar) {
                    rate = seekBar.getProgress();
                    Message message = new Message();
                    message.what = 2;
                    message.arg1 = 1;
                    message.arg2 = rate;
                    mHandler.sendMessage(message);
                }
            });

            SeekBar seekBar2w = findViewById(R.id.light2_white_rate);
            seekBar2w.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                @Override
                public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) { }
                @Override
                public void onStartTrackingTouch(SeekBar seekBar) { }
                @Override
                public void onStopTrackingTouch(SeekBar seekBar) {
                    rate = seekBar.getProgress();
                    Message message = new Message();
                    message.what = 2;
                    message.arg1 = 0;
                    message.arg2 = rate;
                    mHandler.sendMessage(message);
                }
            });

            SeekBar seekBar3y = findViewById(R.id.light3_yellow_rate);
            seekBar3y.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                @Override
                public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) { }
                @Override
                public void onStartTrackingTouch(SeekBar seekBar) { }
                @Override
                public void onStopTrackingTouch(SeekBar seekBar) {
                    rate = seekBar.getProgress();
                    Message message = new Message();
                    message.what = 3;
                    message.arg1 = 1;
                    message.arg2 = rate;
                    mHandler.sendMessage(message);
                }
            });

            SeekBar seekBar3w = findViewById(R.id.light1_white_rate);
            seekBar3w.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                @Override
                public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) { }
                @Override
                public void onStartTrackingTouch(SeekBar seekBar) { }
                @Override
                public void onStopTrackingTouch(SeekBar seekBar) {
                    rate = seekBar.getProgress();
                    Message message = new Message();
                    message.what = 3;
                    message.arg1 = 0;
                    message.arg2 = rate;
                    mHandler.sendMessage(message);
                }
            });


        }
    public class TCPclient implements Runnable
    {
        private static final String serverIP = "192.168.137.206";
        private static final int serverPort = 5005;
        private Socket inetSocket = null;
        private String msg = "110";
        private String rmsg = "110";
        int light = 1;
        int color = 0;
        int rate = 0;

        public Handler getmHandler() {
            return mHandler;
        }

        public void setmHandler(Handler mHandler) {
            this.mHandler = mHandler;
        }

        Handler mHandler = new Handler()
        {
            @Override
            public void handleMessage(@NonNull Message msg) {
                light = msg.what;
                color = msg.arg1;
                rate = msg.arg2;
            }
        };

        @Override
        public void run() {
            try {

                Log.d("TCP","C : Connecting...");
                inetSocket = new Socket(serverIP,serverPort);
                try {
                    Log.d("TCP", "C : sending : ");
                    PrintWriter out = new PrintWriter(new BufferedWriter(
                            new OutputStreamWriter(inetSocket.getOutputStream())),true);

                    while(true) {
                        rmsg = String.valueOf(light) + String.valueOf(color) + String.valueOf(rate);
                        System.out.println("Sending Message : " + rmsg);

                        if(!msg.equals(rmsg)) {
                            out.println(rmsg);
                            msg = rmsg;
                        }
                        System.out.println(String.valueOf(light) + String.valueOf(color) + String.valueOf(rate));
                    }
                } catch (Exception e) {
                    Log.e("TCP", "C: Error1",e);
                }
                finally
                {
                    inetSocket.close();
                }
            } catch (Exception e) {
                Log.e("TCP", "C:Error2",e);
            }
        }
    }
}
