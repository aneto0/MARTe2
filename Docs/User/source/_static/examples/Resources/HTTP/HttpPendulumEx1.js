/**
 * @file HttpPendulumEx1.js 
 * @brief Source file for class HttpPendulumEx1.js
 * @date 28/03/2019
 * @author Andre' Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class HttpPendulumEx1 (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
/**
 * @brief MARTe object browser and display.
 * @details Creates a two pane with a navigation tree on the left and a target display, where to show the selected objects, on the right.
 *
 * The layout of the target display is stored on the localStorage (see getPanelLayout()).
 */
class HttpPendulumEx1 extends MARTeObject {

    /**
     * @brief NOOP
     */
    constructor() {
        super();
    }


    /**
     * @brief Creates the canvas.
     */
    prepareDisplay(target) {
        this.canvas = document.createElement("canvas");
        this.canvas.id = "pendulumCanvas";
        this.canvas.width = 300;
        this.canvas.height = 300;
        this.context = this.canvas.getContext("2d");
        target.innerHTML = "";
        target.appendChild(this.canvas);
        this.lastAngle = 0;
        this.refresh(100);
    }

    /**
     * @brief updates the pendulum with the new angle position.
     * @param[in] jsonData the data as received by the server and which should contain the angle.
     */
    displayData(jsonData) {
        var angle = parseFloat(jsonData["angle"]);
        var rPend = Math.min(this.canvas.width, this.canvas.height) * 0.4;
        var rBall = Math.min(this.canvas.width, this.canvas.height) * 0.04;
        var rBar = Math.min(this.canvas.width, this.canvas.height) * 0.004;
        var ballX = Math.sin(angle) * rPend;
        var ballY = Math.cos(angle) * rPend;
        this.context.fillStyle = "white";
        this.context.fillRect(0, 0, this.canvas.width, this.canvas.height);
        this.context.fillStyle = "yellow";
        this.context.strokeStyle = "black";
        this.context.save();
        this.context.translate(this.canvas.width/2, this.canvas.height/2);
        this.context.rotate(angle);
        this.context.beginPath();
        this.context.rect(-rBar, -rBar, rBar*2, rPend+rBar*2);
        this.context.fill();
        this.context.stroke();
        this.context.beginPath();
        this.context.arc(0, rPend, rBall, 0, Math.PI*2, false);
        this.context.fill();
        this.context.stroke();
        this.context.restore();
        this.lastAngle = angle;
    }
}

