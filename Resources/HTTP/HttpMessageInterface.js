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
        this.map = []
    }
    

    displayMsgRow(table, jsonData, idxN) {
        var idx = ""+idxN
        var tr0 = document.createElement("tr");
        var td0 = document.createElement("td");
        var td01 = document.createElement("td");
        var td02 = document.createElement("td");
        td0.appendChild(document.createTextNode("               "));
        td01.appendChild(document.createTextNode("==== Message ===="));
        td02.appendChild(document.createTextNode("               "));
        tr0.appendChild(td0);
        tr0.appendChild(td01);
        tr0.appendChild(td02);
        table.appendChild(tr0);

        var tr1 = document.createElement("tr");

        var td1 = document.createElement("td");
        td1.appendChild(document.createTextNode(idx));

        var msgBtnTxt = document.createTextNode(jsonData["Name"]);
        var msgBtn = document.createElement("button");
        msgBtn.appendChild(msgBtnTxt);
        var td2 = document.createElement("td");
        td2.appendChild(msgBtn);

        var td3 = document.createElement("td");
        var lastMessageTxt = document.createTextNode("No message sent ever");
        td3.appendChild(lastMessageTxt);        
                
        td1.style.border = "1px solid #000"
        tr1.appendChild(td1);
        tr1.appendChild(td2);
        tr1.appendChild(td3);
        table.appendChild(tr1);

        var paramInfo = jsonData["0"];
        var xInputsTxt=[];
        var xParams=[];
        if (!(paramInfo === undefined)){
            for(var param in paramInfo){
                var tr2 = document.createElement("tr");
                var paramName = document.createTextNode(param);
                var tdx1 = document.createElement("td");
                tdx1.appendChild(paramName);
                tdx1.style.color = "blue";
                tdx1.style.border = "1px solid #000"

                var tdx2 = document.createElement("td");
                if(paramInfo[param][0]=='$'){
                    var inputTxt = document.createElement("input");
                    inputTxt.setAttribute('type', 'text');
                    inputTxt.setAttribute('value', paramInfo[param].substring(1));
                    tdx2.appendChild(inputTxt);
                    xParams.push(param);
                    xInputsTxt.push(inputTxt);
                }
                else{
                    var paramVal = document.createTextNode(paramInfo[param]);
                    tdx2.appendChild(paramVal);
                }
                tr2.appendChild(tdx1);
                tr2.appendChild(tdx2);
                table.appendChild(tr2);
            }
        }
        
        msgBtn.addEventListener("click",
            function(ev, msgBtnTxt, lastMessageTxt, lastMessageTd) {            
                lastMessageTxt.nodeValue = "Sending message";
                lastMessageTd.style.color = "orange";
                var fullURL = MARTeLoader.instance().getDataUrl(this.getPath());
                if (fullURL.includes("?")) {
                    fullURL += ("&msg=" + msgBtnTxt);
                }
                else {
                    fullURL += ("?msg=" + msgBtnTxt);
                }
                for(var x=0; x<xParams.length; x++){
                    fullURL += ("&"+xParams[x]+"=" + xInputsTxt[x].value);
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
            }.bind(this, null, jsonData["Name"], lastMessageTxt, td3),
        false);        
    }


    /**
     * Renders the data on the navigation tree.
     * 
     * @param {obj} jsonData the data as received by the server and which should contain a list of objects.
     */
    displayData(jsonData) {
        var table = document.createElement("table");
        var i = 0;
        var done = false;
        while (!done) {
            var idx = "" + i;
            var msgInfo = jsonData[idx];
            done = (msgInfo === undefined);
            if (!done) {
                this.map.push(msgInfo["Name"]);
                if (msgInfo.hasOwnProperty("IsContainer")){
                    if (msgInfo["IsContainer"]==1){
                        var xhttp = new XMLHttpRequest();
                        var that = this;
                        xhttp.onreadystatechange = function() {
                            if (this.readyState == 4 && this.status == 200) {
                                var msgData = JSON.parse(this.responseText);
                                for(var n=0; n<that.map.length; n++){
                                    if(msgData["Name"]===that.map[n]){
                                        that.displayMsgRow(table, msgData, n);
                                        break;
                                    }
                                }
                            }
                            else {
                                //console.log("Could not communicate with the MARTe server");
                            }
                        };
                        //Get the URL and add all the extra parameters
                        var fullpath = this.getPath();
                        if (!fullpath.endsWith("/")) {
                            fullpath += "/";
                        }
                        var objpath = fullpath + msgInfo["Name"];
                        var fullURL = MARTeLoader.instance().getDataUrl(objpath);
                        xhttp.open("GET", fullURL, true);
                        xhttp.send();                    
                    }
                }
            }
            i++;
        }
        this.target.appendChild(table);
    }
}

