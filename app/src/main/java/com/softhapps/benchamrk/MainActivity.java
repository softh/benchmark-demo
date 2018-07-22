package com.softhapps.benchamrk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    Runnable runnable;
    private Runnable runnable2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv = (TextView) findViewById(R.id.sample_text);

        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Thread thread = new Thread(MainActivity.this.runnable);
                thread.start();

                Thread thread2 = new Thread(MainActivity.this.runnable2);
                thread2.start();

            }
        });


        runnable = new Runnable() {
            public void run() {
                long[] a = getSimplesMillerRabin(1, 100);
                int b = 0;
            }
        };

        runnable2 = new Runnable() {
            public void run() {
                long[] a = getSimplesMillerRabin(100, 200);
                int b = 0;
            }
        };

        //2,147,483,647          2147483647
    }

    public native long[] getSimplesAtkin(long endNumber);

    public native long[] getSimplesAks(long startNumber, long endNumber);

    public native long[] getSimplesMillerRabin(long startNumber, long endNumber);

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
}
