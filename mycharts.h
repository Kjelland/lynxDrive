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

#ifndef MYCHARTS_H
#define MYCHARTS_H
// **************************************************************************
// the includes
#include <QtCharts>
#include <QObject>
#include <QMainWindow>
// **************************************************************************
// the defines

// **************************************************************************
// the typedefs


// **************************************************************************
// the globals

class MyCharts
{
public:
    //! Create an MyChartsClass
    MyCharts();

    //! Refresh all charts
    void refreshChart(double *position,double *speedFB,double *torque,double *speedSP);

    QChart chart;   //!< The chart
/*
    double newTorque=0;

    double newSpeedFeedback=0;

    double newSpeedSetpoint=0;

    double newPosition=0;
*/
    QLineSeries seriesSpeedFeedback; //!< Buffer series for the speed

    QLineSeries seriesTorque;//!< Buffer series for the torque

    QLineSeries seriesSpeedSetpoint;//!< Buffer series for the current

    QLineSeries seriesPosition;//!< Buffer series for the position

    bool showSpeedChart=false;//!< enable the speed chart

    bool showTorqueChart=false;//!< enable the torque chart

    bool showPositionChart=false;//!< enable the position chart

    bool showCurrentChart=false;//!< enable the current chart

    int numberOfSamples=100;//!< number of samples in the chart

private:
    //! \brief     Updates the buffer series with a new value
    //! \param[in] enableChart Enable the function
    //! \param[in] newValue The new value to be inserted into the buffer
    void updateChartSeries(QLineSeries* series,bool enableChart,double newValue);

    int sampleIndex=0;//!< current sample index

    int startIndex=0;//!< current start index

    double chartYMax=1;

    double chartYMin=-1;


};

#endif // MYCHARTS_H

//! \brief     Initializes the central processing unit (CPU) object handle
//! \param[in] pMemory    A pointer to the memory for the CPU object
//! \param[in] numBytes   The number of bytes allocated for the CPU object, bytes
//! \return    The central processing unit (CPU) object handle
