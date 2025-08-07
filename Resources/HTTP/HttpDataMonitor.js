/**
 * @file HttpDataMonitor.js 
 * @date 27/03/2019
 * @author Andre' Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 * Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.
 */
/**
 * Interface to the HttpDataMonitor class.
 */
class HttpDataMonitor extends MARTeObject {

    /**
     * NOOP
     */
    constructor() {
        super();
    }


    /**
     * NOOP
     */
    prepareDisplay(target) {
        this.target = target;
        this.buffer = {};
        this.timestamp = {}
        this.points_1 = 0
        this.divs = [];
        this.inputRefresh = document.createElement('input');
        this.inputRefresh.value = "100";
        var textRefreshRate = document.createTextNode("Refresh rate (ms)");
        target.innerHTML = "";

        this.inputNPoints = document.createElement('input');
        this.inputNPoints.value = "100";
        var textNPoints = document.createTextNode("Number of points");

        this.table = document.createElement("table");
        var tr0 = document.createElement("tr");
        var td00 = document.createElement("td");
        td00.appendChild(textRefreshRate);
        var td01 = document.createElement("td");
        td01.appendChild(this.inputRefresh);
        tr0.appendChild(td00);
        tr0.appendChild(td01);
        var tr1 = document.createElement("tr");
        var td10 = document.createElement("td");
        td10.appendChild(textNPoints);
        var td11 = document.createElement("td");
        td11.appendChild(this.inputNPoints);
        tr1.appendChild(td10);
        tr1.appendChild(td11);
        this.table.appendChild(tr0);
        this.table.appendChild(tr1);
        target.appendChild(this.table);
        this.date = [];

        this.inputRefresh.addEventListener("change",
            function(ev) {
                this.refresh(parseInt(this.inputRefresh.value));
            }.bind(this),
            false);

        var refreshInt = parseInt(this.inputRefresh.value);
        if (!isNaN(refreshInt)) {
            this.refresh(refreshInt);
        }
    }

    /**
     * Renders the data on the navigation tree.
     * 
     * @param {obj} jsonData the data as received by the server and which should contain a list of objects.
     */
    displayData(jsonData) {
        //console.log("%j", jsonData);
        var plots = jsonData["Plots"];
        var cnt = 0;
        for (var plotName in plots) {
            if (this.timestamp[plotName] === undefined) {
                this.timestamp[plotName] = [];
            }

            var plot = plots[plotName]
            var signals = plot["Signals"];
            var points = this.inputNPoints.value;
            var diffPoints = (this.timestamp[plotName].length - points);
            if (diffPoints > 0) {
                this.timestamp[plotName].splice(0, diffPoints + 1);
            }
            else if (diffPoints == 0) {
                this.timestamp[plotName].shift();
            }
            var seconds = plot["__Timestamp"] / 1000.0;
            var minutes = Math.floor(seconds / 60);
            var hours = Math.floor(minutes / 60);
            minutes -= hours * 60;
            var days = Math.floor(hours / 24);
            hours -= days * 24;
            var tstampStr = String(days) + " days - " + String(hours) + " hours - " + String(minutes) + " minutes";
            this.timestamp[plotName].push(seconds);
            if (this.divs.length <= cnt) {
                this.divs.push(document.createElement('div'));
                this.date.push(document.createTextNode(tstampStr));
                this.target.appendChild(this.divs[cnt]);
                this.target.appendChild(this.date[cnt]);
            }
            else {
                this.date[cnt].nodeValue = tstampStr;
            }
            var data = []
            for (var signal in signals) {
                if (Array.isArray(signals[signal])) {
                    for (var n = 0; n < signals[signal].length; n++) {
                        var signalx = signal + "[" + String(n) + "]";
                        if (this.buffer[signalx] === undefined) {
                            this.buffer[signalx] = [];
                        }
                    }
                }
                else {
                    if (this.buffer[signal] === undefined) {
                        this.buffer[signal] = [];
                    }
                }
                if (diffPoints > 0) {
                    if (Array.isArray(signals[signal])) {
                        for (var n = 0; n < signals[signal].length; n++) {
                            var signalx = signal + "[" + String(n) + "]";
                            this.buffer[signalx].splice(0, diffPoints + 1);
                        }
                    }
                    else {
                        this.buffer[signal].splice(0, diffPoints + 1);
                    }
                }
                else if (diffPoints == 0) {
                    if (Array.isArray(signals[signal])) {
                        for (var n = 0; n < signals[signal].length; n++) {
                            var signalx = signal + "[" + String(n) + "]";
                            this.buffer[signalx].shift();
                        }
                    }
                    else {
                        this.buffer[signal].shift();
                    }
                }
                if (Array.isArray(signals[signal])) {
                    for (var n = 0; n < signals[signal].length; n++) {
                        var signalx = signal + "[" + String(n) + "]";
                        this.buffer[signalx].push(signals[signal][n]);
                        data.push({
                            x: this.timestamp[plotName],
                            y: this.buffer[signalx],
                            type: 'scatter',
                            name: signalx,
                            mode: 'lines+markers'
                        });
                    }
                }
                else {
                    this.buffer[signal].push(signals[signal]);
                    data.push({
                        x: this.timestamp[plotName],
                        y: this.buffer[signal],
                        type: 'scatter',
                        name: signal,
                        mode: 'lines+markers'
                    });
                }
            }
            var layout = {
                title: { text: plotName },
                xaxis: {
                    title: {
                        text: 'Time (s)'
                    },
                    showgrid: true
                },
                yaxis: {
                    title: {
                        text: 'Signals'
                    },
                    showgrid: true
                },
                showlegend: true
            };
            Plotly.newPlot(this.divs[cnt], data, layout);
            cnt++;
        }
    }
}
