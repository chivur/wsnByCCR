package com.example.razvan.wsnmonitor;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.TextView;

import java.util.ArrayList;
import com.github.mikephil.charting.*;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

public class Details extends AppCompatActivity {

    ArrayList<String> hist_values;
    String name;
    String currentValue;


    TextView text;

    Button btn_graph;

    LineChart chart;
    RelativeLayout layout;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_details);

        hist_values = new ArrayList<>();
        text = (TextView)findViewById(R.id.tv_details);
        btn_graph = (Button)findViewById(R.id.btn_graph);

        Intent i = getIntent();
        hist_values = i.getStringArrayListExtra("history");
        name = i.getStringExtra("name");
        currentValue = i.getStringExtra("current");

        text.setText(currentValue);

        //layout = (RelativeLayout)findViewById(R.id.graph_layout);
        chart = (LineChart)findViewById(R.id.graph);

        btn_graph.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //layout.addView(chart);
                chart.setDescription("");
                chart.setHighlightPerTapEnabled(true);
                chart.setTouchEnabled(true);
                chart.setDragEnabled(true);
                chart.setScaleEnabled(true);
                chart.setDrawGridBackground(false);
                chart.setPinchZoom(true);
                chart.setBackgroundColor(Color.WHITE);
                chart.setNoDataText("");

                LineData data = new LineData();
                data.setValueTextColor(Color.WHITE);

                chart.setData(data);
                Legend l = chart.getLegend();
                l.setForm(Legend.LegendForm.LINE);
                l.setTextColor(Color.WHITE);

                ArrayList<String> labels = new ArrayList<String>();

                ArrayList<Entry> entries = new ArrayList<Entry>();
                int idx = 0;
                for (String val : hist_values) {
                    entries.add(new Entry(Float.parseFloat(val), idx));
                    labels.add(new String(""+idx));
                    idx++;
                }

                LineDataSet dataset = new LineDataSet(entries, name);
                dataset.setDrawCubic(true);
                data = new LineData(labels,dataset);

                chart.setData(data);

            }
        });



    }

    @Override
    public void onBackPressed(){
        super.onBackPressed();
    }

}
