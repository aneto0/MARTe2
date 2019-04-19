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
 * Alternative plugin renderer for a pendulum.
 */
class HttpPendulumEx1P2 extends MARTeObject {

    /**
     * NOOP
     */
    constructor() {
        super();
    }


    /**
     * Creates the canvas.
     * 
     * @param {obj} target the target HTML container where to display the data.
     */
    prepareDisplay(target) {
        this.targetDivText = document.createElement("div");
        this.targetDivText.setAttribute("class", "divPendulumText");
        target.appendChild(this.targetDivText);
        target.style.background = "#4a4a6b";
        this.refresh(100);
    }

    /**
     * Updates the pendulum with the new angle position.
     *
     * @param {obj} jsonData the data as received by the server and which should contain the angle.
     */
    displayData(jsonData) {
        var angle = parseFloat(jsonData["angle"]);
        this.targetDivText.innerHTML = "" + angle;
    }

    /**
     * Sets the pendulum color.
     * 
     * @param {color} c the new pendulum color.
     */
    setColor(c) {
        this.targetDivText.style.color = c;
    }
}

