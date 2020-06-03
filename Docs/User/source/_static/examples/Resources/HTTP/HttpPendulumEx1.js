/**
 * @file HttpPendulumEx1.js 
 * @date 28/03/2019
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
 * Default plugin renderer for a pendulum.
 */
class HttpPendulumEx1 extends MARTeObject {

    /**
     * NOOP
     */
    constructor() {
        super();
        this.pcolor = "yellow";
    }


    /**
     * Creates the canvas.
     *
     * @param {obj} target the target HTML container where to display the data.
     */
    prepareDisplay(target) {
        this.canvas = document.createElement("canvas");
        this.canvas.setAttribute("class", "pendulumCanvas");
        this.context = this.canvas.getContext("2d");
        target.innerHTML = "";
        target.appendChild(this.canvas);
        this.refresh(100);
    }

    /**
     * Updates the pendulum with the new angle position.
     *
     * @param {obj} jsonData the data as received by the server and which should contain the angle.
     */
    displayData(jsonData) {
        var angle = parseFloat(jsonData["angle"]);
        var width = this.canvas.clientWidth;
        var height = this.canvas.clientHeight;
        var length = Math.min(width, height);
        var rPend = length * 0.4;
        var rBall = length * 0.04;
        var rBar = length * 0.004;
        var ballX = Math.sin(angle) * rPend;
        var ballY = Math.cos(angle) * rPend;
        this.context.fillStyle = "white";
        this.context.fillRect(0, 0, width, height);
        this.context.save();
        this.context.translate(width/2, 10);
        this.context.rotate(angle);
        this.context.beginPath();
        this.context.rect(-rBar, -rBar, rBar*2, rPend+rBar*2);
        this.context.strokeStyle = "black";
        this.context.fillStyle = "black";
        this.context.fill();
        this.context.stroke();
        this.context.beginPath();
        this.context.arc(0, rPend, rBall, 0, Math.PI*2, false);
        this.context.fillStyle = this.pcolor;
        this.context.fill();
        this.context.stroke();
        this.context.restore();
    }

    /**
     * Sets the pendulum color.
     *
     * @param {color} c the new pendulum color.
     */
    setColor(c) {
        this.pcolor = c;
    }
}

