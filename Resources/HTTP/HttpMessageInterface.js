/**
 * @file HttpMessageInterface.js 
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
 * Interface to the HttpMessageInterface class. Creates one button for each Message.
 */
class HttpMessageInterface extends MARTeObject {

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
    }

    /**
     * Renders the data on the navigation tree.
     * 
     * @param {obj} jsonData the data as received by the server and which should contain a list of objects.
     */
    displayData(jsonData) {
        var i = 0;
        var done = false;
        var table = document.createElement("table");
        while (!done) {
            var idx = "" + i;
            var msgInfo = jsonData[idx];
            done = (msgInfo === undefined);
            if (!done) {
                var tr = document.createElement("tr");
                var td1 = document.createElement("td");
                td1.appendChild(document.createTextNode(idx));
                var td2 = document.createElement("td");
                var td3 = document.createElement("td");
                var msgBtn = document.createElement("button");
                var msgBtnTxt = document.createTextNode(msgInfo["Name"]);
                msgBtn.appendChild(msgBtnTxt);
                td2.appendChild(msgBtn);
                
                var lastMessageTxt = document.createTextNode("No message sent ever");
                
                tr.appendChild(td1);
                tr.appendChild(td2);
                tr.appendChild(td3);
                td3.appendChild(lastMessageTxt);
                table.appendChild(tr);

                msgBtn.addEventListener("click",
                    function(ev, msgBtnTxt, lastMessageTxt, lastMessageTd) {			
                        var fullURL = MARTeLoader.instance().getDataUrl(this.getPath());
                        if (fullURL.includes("?")) {
                            fullURL += ("&msg=" + msgBtnTxt);
                        }
                        else {
                            fullURL += ("?msg=" + msgBtnTxt);
                        }
                        var xhttp = new XMLHttpRequest();
                        var that = this;
                        xhttp.onreadystatechange = function() {
                            if (this.readyState == 4 && this.status == 200) {
                                try {
                                    var jsonData = JSON.parse(this.responseText);
                                    var ok = jsonData["OK"];
                                    if (ok !== undefined) {
                                        ok = parseInt(ok);
                                    }
                                    else {
                                        ok = 0;
                                    }
                                    if (ok === 1) {
                                        lastMessageTxt.nodeValue = "Last message was successfully sent";
                                        lastMessageTd.style.color = "green";
                                    }
                                    else {
                                        lastMessageTxt.nodeValue = "Last message was not successfully sent";
                                        lastMessageTd.style.color = "red";
                                    }
                                }
                                catch (e) {
                                    console.log(e);
                                }
                            }
                        };	
                        xhttp.open("GET", fullURL, true);
                        xhttp.send();
                    }.bind(this, null, msgInfo["Name"], lastMessageTxt, td3),
                    false);
            }
            i++;
        }
        this.target.appendChild(table);
    }
}

