/* --COPYRIGHT--,BSD
 * Copyright (c) 2018, Lynx Dynamics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Lynx Dynamics nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
// **************************************************************************
// the includes

#include "mycharts.h"

// **************************************************************************
// the defines

// **************************************************************************
// the typedefs


// **************************************************************************
// the globals


// **************************************************************************
// the functions


MyCharts::MyCharts()
{
    seriesTorque.setName("Torque");
    QPen pen("Red");
    pen.setWidth(3);
    seriesTorque.setPen(pen);

    seriesSpeedFeedback.setName("Speed Feedback");
    pen.setColor("Green");
    seriesSpeedFeedback.setPen(pen);

    seriesSpeedSetpoint.setName("Speed Setpoint");
    pen.setColor("Blue");
    seriesSpeedSetpoint.setPen(pen);

    seriesPosition.setName("Position");
    pen.setColor("Brown");
    seriesPosition.setPen(pen);
    //chart.addSeries(&seriesCurrent);
    //chart.setTitle("Simple areachart example");
}

void MyCharts::updateChartSeries(QLineSeries* series,bool enableChart,double newValue)
{
    if(enableChart)
    {
        if(series->count() > numberOfSamples)
        {
             series->removePoints(0,1);
        }
        series->append(sampleIndex, newValue);

        if(newValue>chartYMax)
            chartYMax=(newValue);
        if(newValue<chartYMin)
            chartYMin=(newValue);
    }

}


void MyCharts::refreshChart(double *position,double *speed,double *torque,double *current)
{
    //Update chart
    if(showTorqueChart||showSpeedChart||showPositionChart||showCurrentChart)
    {
        if(sampleIndex>numberOfSamples)
            startIndex=sampleIndex-numberOfSamples;
        //Generate random data..
        updateChartSeries(&seriesTorque,showTorqueChart,*torque);
        updateChartSeries(&seriesSpeedFeedback,showSpeedChart,*speed);
        updateChartSeries(&seriesSpeedSetpoint,showCurrentChart,*current);
        updateChartSeries(&seriesPosition,showPositionChart,*position);
        chart.axisX()->setRange(startIndex,sampleIndex);
        chart.axisY()->setRange((1.2*chartYMin),1.2*chartYMax);
        sampleIndex++;
    }
    else
    {
        sampleIndex=0;
        startIndex=0;
    }

}
