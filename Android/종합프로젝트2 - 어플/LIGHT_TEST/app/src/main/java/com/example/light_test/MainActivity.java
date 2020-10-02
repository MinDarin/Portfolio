package com.example.light_test;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.BufferedWriter;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;

import static java.lang.Thread.sleep;

public class MainActivity extends AppCompatActivity {
    TCPclient tcpThread = new TCPclient();
    Handler mHandler = tcpThread.getmHandler();
    String passwd="";
    int num = 0;
    @Override

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        final EditText showpasswd1 = (EditText)findViewById(R.id.ShowPasswd1);
        final EditText showpasswd2 = (EditText)findViewById(R.id.ShowPasswd2);
        final EditText showpasswd3 = (EditText)findViewById(R.id.ShowPasswd3);
        final EditText showpasswd4 = (EditText)findViewById(R.id.ShowPasswd4);

        final Button Btn1 = (Button)findViewById(R.id.button1);
        Btn1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                passwd += "1";
                num++;
                switch (num)
                {
                    case 1:
                        showpasswd1.setText("1");
                        break;
                    case 2:
                        showpasswd2.setText("1");
                        break;

                    case 3:
                        showpasswd3.setText("1");
                        break;

                    case 4:
                        showpasswd4.setText("1");
                        break;
                }
            }
        });

        final Button Btn2 = (Button)findViewById(R.id.button2);
        Btn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                passwd += "2";
                num++;
                switch (num)
                {
                    case 1:
                        showpasswd1.setText("2");
                        break;
                    case 2:
                        showpasswd2.setText("2");
                        break;
                    case 3:
                        showpasswd3.setText("2");
                        break;
                    case 4:
                        showpasswd4.setText("2");
                        break;
                }
            }
        });


        final Button Btn3 = (Button)findViewById(R.id.button3);
        Btn3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                passwd += "3";
                num++;
                switch (num)
                {
                    case 1:
                        showpasswd1.setText("3");
                        break;
                    case 2:
                        showpasswd2.setText("3");
                        break;
                    case 3:
                        showpasswd3.setText("3");
                        break;
                    case 4:
                        showpasswd4.setText("3");
                        break;
                }

            }
        });


        final Button Btn4 = (Button)findViewById(R.id.button4);
        Btn4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                passwd += "4";
                num++;
                switch (num)
                {
                    case 1:
                        showpasswd1.setText("4");
                        break;
                    case 2:
                        showpasswd2.setText("4");
                        break;
                    case 3:
                        showpasswd3.setText("4");
                        break;
                    case 4:
                        showpasswd4.setText("4");
                        break;
                }

            }
        });

        final Button Btn5 = (Button)findViewById(R.id.button5);
        Btn5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                passwd += "5";
                num++;
                switch (num)
                {
                    case 1:
                        showpasswd1.setText("5");
                        break;
                    case 2:
                        showpasswd2.setText("5");
                        break;
                    case 3:
                        showpasswd3.setText("5");
                        break;
                    case 4:
                        showpasswd4.setText("5");
                        break;
                }

            }
        });

        final Button Btn6 = (Button)findViewById(R.id.button6);
        Btn6.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                passwd += "6";
                num++;
                switch (num)
                {
                    case 1:
                        showpasswd1.setText("6");
                        break;
                    case 2:
                        showpasswd2.setText("6");
                        break;
                    case 3:
                        showpasswd3.setText("6");
                        break;
                    case 4:
                        showpasswd4.setText("6");
                        break;
                }


            }
        });

        final Button Btn7 = (Button)findViewById(R.id.button7);
        Btn7.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                passwd += "7";
                num++;
                switch (num)
                {
                    case 1:
                        showpasswd1.setText("7");
                        break;
                    case 2:
                        showpasswd2.setText("7");
                        break;
                    case 3:
                        showpasswd3.setText("7");
                        break;
                    case 4:
                        showpasswd4.setText("7");
                        break;
                }

            }
        });

        final Button Btn8 = (Button)findViewById(R.id.button8);
        Btn8.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                passwd += "8";
                num++;
                switch (num)
                {
                    case 1:
                        showpasswd1.setText("8");
                        break;
                    case 2:
                        showpasswd2.setText("8");
                        break;
                    case 3:
                        showpasswd3.setText("8");
                        break;
                    case 4:
                        showpasswd4.setText("8");
                        break;
                }

            }
        });

        final Button Btn9 = (Button)findViewById(R.id.button9);
        Btn9.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                passwd += "9";
                num++;
                switch (num)
                {
                    case 1:
                        showpasswd1.setText("9");
                        break;
                    case 2:
                        showpasswd2.setText("9");
                        break;
                    case 3:
                        showpasswd3.setText("9");
                        break;
                    case 4:
                        showpasswd4.setText("9");
                        break;
                }

            }
        });

        final Button Btn0 = (Button)findViewById(R.id.button0);
        Btn0.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                passwd += "0";
                num++;
                switch (num)
                {
                    case 1:
                        showpasswd1.setText("0");
                        break;
                    case 2:
                        showpasswd2.setText("0");
                        break;
                    case 3:
                        showpasswd3.setText("0");
                        break;
                    case 4:
                        showpasswd4.setText("0");
                        break;
                }

            }
        });

        final Button BtnOK = (Button)findViewById(R.id.OK);
        BtnOK.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v)
            {
                System.out.println(passwd);

                if (passwd.equals("0000"))
                {
                    Message message = new Message();
                    message.what = 5;
                    message.arg1 = 0;
                    message.arg2 = 0;
                    mHandler.sendMessage(message);
                    Toast toast  = Toast.makeText(getApplicationContext(),"환영합니다.",Toast.LENGTH_LONG);
                    toast.setGravity(Gravity.TOP,10,190);
                    toast.show();

                    Intent intent = new Intent(getApplicationContext(), SubActivity.class);
                   startActivity(intent);

                }
                else
                {
                    Toast toast  = Toast.makeText(getApplicationContext(),"비밀번호가 틀렸습니다.",Toast.LENGTH_LONG);
                    toast.setGravity(Gravity.TOP,10,190);
                    toast.show();
                    passwd = "";
                    num = 0;
                    showpasswd1.setText("");
                    showpasswd2.setText("");
                    showpasswd3.setText("");
                    showpasswd4.setText("");
                }
        }
        });
        final Button BtnLock = (Button)findViewById(R.id.lock);
        BtnLock.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v)
            {
                System.out.println(passwd);
                num = 0;
                showpasswd1.setText("");
                showpasswd2.setText("");
                showpasswd3.setText("");
                showpasswd4.setText("");

            }
        });
        final Button BtnCancel = (Button)findViewById(R.id.Cancel);
        BtnCancel.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v)
            {
                num = 0;
                showpasswd1.setText("");
                showpasswd2.setText("");
                showpasswd3.setText("");
                showpasswd4.setText("");

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