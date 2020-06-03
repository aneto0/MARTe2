/**
 * @file HttpObjectBrowser.js 
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
 * MARTe object browser and display.
 * 
 * Creates a two pane with a navigation tree on the left and a target display, where to show the selected objects, on the right.
 *
 * The layout of the target display is stored on the localStorage (see getPanelLayout()).
 */
class HttpObjectBrowser extends MARTeObject {

    /**
     * NOOP
     */
    constructor() {
        super();
    }


    /**
     * Creates the navigation tree and the target display.
     * 
     * @param {obj} target the target container where to display the data.
     */
    prepareDisplay(target) {
        this.uniquePanelId = new Date().getTime();	
        target.innerHTML = "";
        this.target = target;
        this.mainRow = document.createElement("div");
        this.leftPaneContainer = document.createElement("div");        
        this.leftPaneTree = document.createElement("div");
        this.leftPaneConfigHeader = document.createElement("div");
        this.leftPaneConfigPanel = document.createElement("div");        

        this.rightPaneContainer = document.createElement("div");

        var showConfigBtn = document.createElement("button");
        var showConfigBtnTxt = document.createTextNode("*");
        showConfigBtn.appendChild(showConfigBtnTxt);
        showConfigBtn.addEventListener("click",
                function(ev) {			
                    this.toggleConfig();				
                }.bind(this),
                false);

        this.mainRow.setAttribute("class", "mainRow");
        this.leftPaneContainer.setAttribute("class", "mainColumnNav");
        this.leftPaneTree.setAttribute("class", "mainColumnTree");
        this.leftPaneConfigHeader.setAttribute("class", "mainColumnConfig");
        this.leftPaneConfigPanel.setAttribute("class", "mainPanelConfig");
        this.leftPaneConfigPanel.style.display = "none";
        this.addConfigOptions();

        showConfigBtn.setAttribute("class", "configButton");
        this.rightPaneContainer.setAttribute("class", "mainColumnTarget");


        this.leftPaneConfigHeader.appendChild(showConfigBtn);				
        this.leftPaneContainer.appendChild(this.leftPaneConfigHeader);
        this.leftPaneContainer.appendChild(this.leftPaneTree);		
        this.leftPaneContainer.appendChild(this.leftPaneConfigPanel);
        this.mainRow.appendChild(this.leftPaneContainer);
        this.mainRow.appendChild(this.rightPaneContainer); 
        this.target.appendChild(this.mainRow);		 
        this.createTargetPanels(this.getPanelLayout());

        this.panelsConfigurationText = document.getElementById("panelsConfigurationText");
        var saveConfigButton = document.getElementById("saveConfig");
        saveConfigButton.addEventListener("click",
                function(ev) {
                    try {
                        var newConfiguration = JSON.parse(this.panelsConfigurationText.value);
                        localStorage[this.getConfigName()] = this.panelsConfigurationText.value;
                        window.open(window.location.href);				
                    }
                    catch (error) {
                        alert("Invalid layout: " + error);
                    }
                }.bind(this),
                false);

    }

    /**
     * Creates the configuration panel.
     */
    addConfigOptions() {
        this.leftPaneConfigPanel.innerHTML = "<table class=\"mainPanelConfigTable\">" + 
            "<th colspan=\"2\" class=\"mainPanelConfigTableTh\">Configuration</th>" +
            "<tr>" +
            "<td>Layout</td>" +
            "<td><input type=\"text\" id=\"panelsConfigurationText\" name=\"panelsConfigurationText\" style=\"width:100%\"></td>" +
            "</tr><tr>" +
            "<td colspan=\"2\"><button id=\"saveConfig\" style=\"float:right\">Save/Apply</button></td>" +
            "</tr>" +
            "</table>";
    }

    /**
     * Shows/hides the configuration panel.
     */
    toggleConfig() {
        if (this.leftPaneConfigPanel.style.display === "none") {
            this.panelsConfigurationText.value = JSON.stringify(this.getPanelLayout());
            this.leftPaneConfigPanel.style.display = "flex";
        }
        else {
            this.leftPaneConfigPanel.style.display = "none";
        }
    }

    /**
     * Renders the data on the navigation tree.
     * @param {obj} jsonData the data as received by the server and which should contain a list of objects.
     */
    displayData(jsonData) {
        this.createTreeNodes(jsonData, "", this.leftPaneTree);
        this.registerTreeLinks(jsonData, "");
    }

    /**
     * Helper method which verifies if a given node is open.
     * 
     * @param {obj} btn the node to be checked.
     *
     * @return {boolean} true if the node is open (i.e. expanded), false otherwise.
     */
    isNodeOpen(btn) {
        return (btn.innerHTML === "-");
    }

    /**
     * Helper method which verifies if a given node is already populated (otherwise the required information will be retrieved from the server - only once).
     * 
     * @param {obj} node the node to be checked.
     *
     * @return {boolean} true if the node is already populated.
     */
    isNodePopulated(node) {
        //two buttons
        return (node.children.length > 2);
    }

    /**
     * Helper method which expands a given tree node. Note that the childs nodes are only populated once.
     * 
     * @param {string} path the full object path.
     * @param {obj} btn the button to expand.
     * @param {obj} parentNode the node where to append the new nodes.
     */
    expandTreeNode(path, btn, parentNode) {
        if (!this.isNodeOpen(btn)) {
            if (!this.isNodePopulated(parentNode)) {
                var xhttp = new XMLHttpRequest();
                var that = this;
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        var jsonData = JSON.parse(this.responseText);
                        that.createTreeNodes(jsonData, path, parentNode);
                        that.registerTreeLinks(jsonData, path);
                    }
                    else {
                        //console.log("Could not communicate with the MARTe server");
                    }
                };
                //Get the URL and add all the extra parameters
                var fullURL = MARTeLoader.instance().getDataUrl(path);
                xhttp.open("GET", fullURL, true);
                xhttp.send();
                btn.innerHTML = "-";
            }
            else {
                //Find the inner ul element
                var ul = parentNode.querySelector(".nested");
                if (ul !== null) {
                    ul.classList.add("active");
                    btn.innerHTML = "-";
                }
            }
        }
        else {
            var ul = parentNode.querySelector(".nested");
            if (ul !== null) {
                ul.classList.remove("active");
                btn.innerHTML = "+";
            }
            btn.innerHTML = "+";
        }
    }

    /**
     * Shows the select tree node in the target container or in a new page.
     * @param {string} path the full object path.
     * @param {obj} obj the object to show.
     * @param {string} rightPaneContainerId the identifier of the target HTML element.
     */
    showTreeNode(path, obj, rightPaneContainerId) {
        var marteClassName = obj.getAttribute("marteClassName");
        if (marteClassName !== null) {
            if (rightPaneContainerId.length > 0) {
                MARTeLoader.instance().load(path, marteClassName, rightPaneContainerId);
            }
            else {
                var url = "?TextMode=1&ObjPath=" + path;
                window.open(url, '_blank');				
            }
        }		
    }

    /**
     * Helper method to replace all the instance of / with a _ on a given identifier.
     * 
     * @param {string} id the identifier to be replaced.
     * 
     * @return {string} the modified identifier.
     */
    generateId(id) {
        return id.replace(/\//g, '_');
    }

    /**
     * Helper method to append navigation tree nodes to a parent node.
     *
     * @param {obj} jsonData list of objects as received from the server.
     * @param {string} fullpath the complete path to this instance HttpObjectBrowser instance.
     * @param {obj} parentNode the node where the new nodes will be added to.
     */
    createTreeNodes(jsonData, fullpath, parentNode) {
        var i = 0;
        var panelConfig = this.getPanelLayout();
        var nodeName = "" + i;
        var hasMoreNodes = (jsonData[nodeName] !== undefined);
        if (fullpath.length === 0)  {
            fullpath = this.getPath();
        }
        if (fullpath.length > 0) {
            if (!fullpath.endsWith("/")) {
                fullpath += "/";
            }
        }		

        var ul = document.createElement("ul");
        parentNode.appendChild(ul);
        ul.setAttribute("class", "nested active");
        parentNode = ul;
        while (hasMoreNodes) {
            var child = jsonData[nodeName];
            var objName = child["Name"];
            var objpath = fullpath + objName;
            var txt = objName;
            var marteClassName = child["Class"];
            txt += " (" + marteClassName + ")";
            var pathid = this.generateId(objpath);

            var isContainer = child["IsContainer"];
            if (isContainer !== undefined) {
                isContainer = (isContainer === 1);
            }
            else {
                isContainer = false;
            }

            var expandBtn = document.createElement("button");
            expandBtn.setAttribute("class", "treeButton");
            var expandBtnTxt = document.createTextNode("+");
            if (isContainer) {
                var expandBtnId = "ebtn_" + pathid;
                expandBtn.setAttribute("id", expandBtnId);
            }
            else {
                expandBtn.disabled = true;
            }
            expandBtn.appendChild(expandBtnTxt);

            var showObjectBtn = document.createElement("button");
            showObjectBtn.setAttribute("class", "treeButton");
            var showObjectBtnTxt = document.createTextNode(">");
            var showObjectBtnId = "sbtn_" + pathid;
            showObjectBtn.setAttribute("id", showObjectBtnId);
            showObjectBtn.setAttribute("class", "dropButton");
            showObjectBtn.appendChild(showObjectBtnTxt);

            var li = document.createElement("li");
            var liid = "li_" + pathid;
            li.setAttribute("id", liid);
            li.setAttribute("marteClassName", marteClassName);
            li.appendChild(expandBtn);
            var litxt = document.createTextNode(" " + txt + " ");
            li.appendChild(litxt);

            var divParentContainer = document.createElement("div");
            divParentContainer.setAttribute("class", "dropdown");			

            divParentContainer.appendChild(showObjectBtn);

            var dropdownDivContainers = document.createElement("div");
            dropdownDivContainers.setAttribute("class", "dropdown-content");
            var nrows = panelConfig.length;
            for (var r=0; r<nrows; r++) {
                var ncols = panelConfig[r].length;
                for (var c=0; c<ncols; c++) {
                    var divContainerLink = document.createElement("a");
                    var rightPaneContainerId = this.getRightPaneContainerId(r, c);
                    if (document.getElementById(rightPaneContainerId)) {
                        var divContainerLinkId = showObjectBtnId + rightPaneContainerId;					
                        divContainerLink.setAttribute("id", divContainerLinkId);
                        var divContainerLinkTxt = document.createTextNode(r + "x" + c);
                        divContainerLink.appendChild(divContainerLinkTxt);
                        dropdownDivContainers.appendChild(divContainerLink);
                    }
                }
            }
            {
                //New link
                var divContainerLink = document.createElement("a");
                var rightPaneContainerId = this.getRightPaneContainerId(r, c);
                var divContainerLinkId = showObjectBtnId + "_New";					
                divContainerLink.setAttribute("id", divContainerLinkId);
                var divContainerLinkTxt = document.createTextNode("N");
                divContainerLink.appendChild(divContainerLinkTxt);
                dropdownDivContainers.appendChild(divContainerLink);
            }
            divParentContainer.appendChild(dropdownDivContainers);								
            li.appendChild(divParentContainer);
            parentNode.appendChild(li);

            i++;
            nodeName = "" + i;
            hasMoreNodes = (jsonData[nodeName] !== undefined);
        }
    }

    /**
     * Helper method to register the links on the navigation tree.
     * @param {obj} jsonData list of objects as received from the server.
     * @param {string} fullpath the complete path to this instance HttpObjectBrowser instance.
     */
    registerTreeLinks(jsonData, fullpath) {
        var i = 0;
        var nodeName = "" + i;
        var panelConfig = this.getPanelLayout();

        var hasMoreNodes = (jsonData[nodeName] !== undefined);
        if (fullpath.length === 0)  {
            fullpath = this.getPath();
        }
        if (fullpath.length > 0)  {
            if (!fullpath.endsWith("/")) {
                fullpath += "/";
            }
        }		

        while (hasMoreNodes) {
            var child = jsonData[nodeName];
            var objName = child["Name"];			
            var objpath = fullpath + objName;

            var pathid = this.generateId(fullpath + objName);
            var expandObjectBtnId = "ebtn_" + pathid;
            var liid = "li_" + pathid;
            var li = document.getElementById(liid);
            var expandObjectBtn = document.getElementById(expandObjectBtnId);
            if (expandObjectBtn !== null) {				
                expandObjectBtn.addEventListener("click",
                        function(ev, objpath, expandObjectBtn, li) {						
                            this.expandTreeNode(objpath, expandObjectBtn, li);
                            //null is the event that I don't really care about, objpath is to remember the path that was set while doing the loop (otherwise it will always get the last value set for objpath)
                        }.bind(this, null, objpath, expandObjectBtn, li),
                        false);
            }


            var showObjectBtnId = "sbtn_" + pathid;

            var nrows = panelConfig.length;
            for (var r=0; r<nrows; r++) {
                var ncols = panelConfig[r].length;
                for (var c=0; c<ncols; c++) {	
                    var rightPaneContainerId = this.getRightPaneContainerId(r, c);
                    if (document.getElementById(rightPaneContainerId)) {
                        var divContainerLinkId = showObjectBtnId + rightPaneContainerId;			
                        var divContainerLink = document.getElementById(divContainerLinkId);
                        divContainerLink.addEventListener("click",
                                function(ev, objpath, li, rightPaneContainerId) {
                                    this.showTreeNode(objpath, li, rightPaneContainerId);
                                    //null is the event that I don't really care about, objpath is to remember the path that was set while doing the loop (otherwise it will always get the last value set for objpath)
                                }.bind(this, null, objpath, li, rightPaneContainerId),
                                false);
                    }
                }
            }
            {
                var divContainerLinkId = showObjectBtnId + "_New";			
                var divContainerLink = document.getElementById(divContainerLinkId);
                divContainerLink.addEventListener("click",
                        function(ev, objpath, li) {
                            this.showTreeNode(objpath, li, '');
                            //null is the event that I don't really care about, objpath is to remember the path that was set while doing the loop (otherwise it will always get the last value set for objpath)
                        }.bind(this, null, objpath, li, rightPaneContainerId),
                        false);
            }
            i++;
            nodeName = "" + i;
            hasMoreNodes = (jsonData[nodeName] !== undefined);
        }
    }

    /**
     * Generates a unique identified based on a given row and column.
     * @param {int} row the row.
     * @param {int} col the column.
     * @return {string} the unique identifier.
     */
    getRightPaneContainerId(row, col) {
        var rcid;
        if (col == undefined) {
            rcid = "_" + this.uniquePanelId + "_rightPaneContainerR" + row; 
        }
        else {
            rcid = "_" + this.uniquePanelId + "_rightPaneContainerR" + row + "C" + col;
        }
        return rcid;
    }


    /**
     * Helper method to create the target panel as per the getPanelLayout().
     * @param {obj} config the layout configuration.
     */
    createTargetPanels(config) {
        var target = this.rightPaneContainer;
        var nrows = config.length;
        var rheight = 100 / nrows; 

        for (var r=0; r<nrows; r++) {
            var ncols = config[r].length;
            //Add the row
            var rowd = document.createElement("div"); 
            var rid = this.getRightPaneContainerId(r); 
            rowd.setAttribute("class", "mainRowTargetItem");
            rowd.setAttribute("id", rid);
            rowd.style.height = rheight + "%";

            target.appendChild(rowd); 			
            for (var c=0; c<ncols; c++) {
                //Add the columns
                var cold = document.createElement("div"); 
                var cid = this.getRightPaneContainerId(r, c);
                var cwidth = config[r][c]; 				 
                cold.setAttribute("class", "mainColumnTargetItem");
                cold.setAttribute("id", cid);
                cold.style.width = cwidth + "%";
                if (c == (ncols - 1)) {
                    cold.style.resize = 'none';
                }

                var logo = document.getElementById("martelogo").cloneNode(true);
                logo.removeAttribute("hidden");
                cold.appendChild(logo);
                rowd.appendChild(cold);
            }
        }
    }

    /**
     * Gets the panel layout as stored in the localStorage.
     * 
     * @return {obj} the panel config as a JSON structure.
     */
    getPanelLayout() {
        var configName = this.getConfigName();
        if (localStorage[configName] === undefined) {
            localStorage[configName] = JSON.stringify([[100], [100]]);
        }
        var panelConfig = JSON.parse(localStorage[configName]);
        return panelConfig;
    }

    /**
     * Gets the name of the configuration string (for the localStorage) associated to the current instance.
     * @return {string} the configuration string identifier.
     */
    getConfigName() {
        var getVars = {};
        window.location.href.replace(/[?&]+([^=&]+)=([^&]*)/gi, function(m, key, value) {
            getVars[key] = value;
        });
        var objpath = getVars["ObjPath"];
        if (objpath === undefined) {
            objpath = "";
        }
        objpath.replace(/\//g, "_");
        var configName = "HttpObjectBrowser_" + objpath;
        return configName; 
    }
}

