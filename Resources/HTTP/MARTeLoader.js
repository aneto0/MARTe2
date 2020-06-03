/**
 * @file MARTeLoader.js 
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
 * Helper class that allow to load javascript and css code based on a given class name.
 */
class MARTeLoader {

    /**
     * Singleton access to the Loader class.
     * 
     * @return {obj} the singleton of the MARTeLoader.
     */
    static instance() {
        if (this.ml === undefined) {
            this.ml = new MARTeLoader();
            this.ml.urlCache = {};
            this.ml.loading = false;
            this.ml.head = document.getElementsByTagName('head')[0];
        }
        return this.ml;
    }

    /**
     * Only executes the callback if the url exists in the server. 
     * 
     * @param {string} url the url to be checked.
     * @param {function} callbackOK the function callback to be called if the resource exists in the server.
     * @param {function} callbackFailed the function callback to be called if the resource does NOT exist in the server.
     */
    executeIffUrlExists(url, callbackOK, callbackFailed) {
        if (this.urlCache[url] === undefined) {
            var xhttp = new XMLHttpRequest();
            var that = this;
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    callbackOK();
                    that.urlCache[url] = true;
                }
                else if (this.status == 400 || this.status == 404) {
                    if (callbackFailed !== undefined) {
                        callbackFailed();
                    }
                    that.urlCache[url] = false;
                }
            };
            xhttp.open("HEAD", url, true);
            xhttp.send();
        }
        else {
            if (this.urlCache[url]) {
                callbackOK();
            }
            else {
                if (callbackFailed !== undefined) {
                    callbackFailed();
                }
            }
        }
    }

    /**
     * Gets the javascript plugin url from a class name.
     *
     * @param {string} className the class name for the plugin.
     *
     * @return {string} the url where the javascript plugin for that class name might exist.
     */
    getPluginJsUrl(className) {
        var url = "/?path=" + className + ".js&TextMode=1";
        return url;
    }

    /**
     * Gets the CSS plugin url from a class name.
     * 
     * @param {string} className the class name for the plugin.
     *
     * @return {string} the url where the CSS plugin for that class name might exist.
     */
    getPlugiCssUrl(className) {
        var url = "/?path=" + className + ".css&TextMode=1";
        return url;
    }

    /**
     * Reconstructs the URL associated to a given object path.
     *
     * @param {string} fullPath the object path (e.g. A/B/C)
     *
     * @return {string} the URL associated to the object path, e.g. http://localhost:9094/A/B/C?Param1=1&TextMode=1
     */
    getDataUrl(fullPath) {
        //Get the URL and add all the extra parameters
        var pname = window.location.pathname;
        var getparams = window.location.search;
        if (getparams.includes("TextMode=")) {
            getparams = getparams.replace("TextMode=1", "TextMode=0");
        }
        else {
            if (getparams.length === 0) {
                getparams += "?";
            }
            else {
                getparams += "&";
            }
            getparams += "TextMode=0";
        }
        var fullUrl = pname;
        if (fullPath.length > 0) {
            if (!fullUrl.endsWith("/")) {
                fullUrl += "/";
            }
            fullUrl += fullPath;
        }
        fullUrl += getparams;
        return fullUrl;
    }	

    /**
     * Discovers and loads the css and javascript associated to a given class name (that inherits from MARTeObject).
     * 
     * @param {string} fullPath the object path (e.g. A/B/C).
     * @param {string} className the name of the class (which shall inherit from MARTeObject) (e.g. HttpObjectBrowser). If the className is not specified, or has 0 length, then the class name is automatically set to be the "Class" field in the json structure which retrieved from the server using TextMode = 0 (with the URL=fullPath).
     * @param {string} containerId the HTML identifier of the container where the target plugin should be load into.
     * @param {function} objLoadedCallback callback function to be called when the object has been loaded.
     */
    load(fullPath, className, containerId, objLoadedCallback) {
        var xhttp = new XMLHttpRequest();
        var that = this;
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                var containerHtmlElem = document.getElementById(containerId);
                var jsonData;
                try {
                    jsonData = JSON.parse(this.responseText);
                    if (className.length === 0) {
                        className = jsonData["Class"];
                    }
                    if (className !== undefined) {
                        //Check if the url for the plugin exists
                        var jsUrl = that.getPluginJsUrl(className);
                        var cssUrl = that.getPlugiCssUrl(className);
                        that.executeIffUrlExists(cssUrl, function() {
                            that.loadCSS(className);
                        });
                        that.executeIffUrlExists(jsUrl,
                            //If there is a javascript try to load it.
                            function() {
                                that.loadJS(fullPath, className, jsonData, containerId, objLoadedCallback);
                            }.bind(this),
                            //Otherwise just show the raw data.
                            function() {
                                if (this.standardDisplay === undefined) {
                                    this.standardDisplay = new MARTeObject();
                                    this.standardDisplay.setPath(fullPath);
                                    containerHtmlElem.innerHTML = "";
                                    this.standardDisplay.prepareDisplay(containerHtmlElem);
                                    this.standardDisplay.displayData(jsonData);
                                }
                            }.bind(this),
                        );
                    }
                    else {
                        console.log("The className was not found!");
                    }
                }
                catch (e) {
                    var textarea = document.createElement('textarea');
                    textarea.setAttribute("disabled", true);
                    textarea.innerHTML = "Failed to parse json from server " + e;
                    containerHtmlElem.innerHTML = "";
                    containerHtmlElem.appendChild(textarea);
                }
            }
        };
        //Get the URL and add all the extra parameters
        var fullURL = this.getDataUrl(fullPath);
        xhttp.open("GET", fullURL, true);
        xhttp.send();
    }

    /**
     * Checks if the resource was already added to the head section of the document.
     * @param {string} marteClassName the class name that identifies the resource.
     *
     * @return {boolean} true if the resource already exists.
     */
    resourceAlreadyExists(marteClassName) {		
        var scriptAlreadyExists = false;
        var nChildren = this.head.children.length;
        for (var c = 0; (c < nChildren) && (!scriptAlreadyExists); c++) {
            var src = this.head.children[c].getAttribute("src");
            if (src !== null) {
                scriptAlreadyExists = src.includes(marteClassName);
            }
        }
        return scriptAlreadyExists;
    }

    /**
     * Loads the javascript associated to the className and if successful updates the target HTML container.
     *
     * @param {string} fullPath the object path (e.g. A/B/C).
     * @param {string} className the name of the class (e.g. HttpObjectBrowser).
     * @param {obj} jsonData json data as received from the server and that should be offered to the plugin (see jsLoaded).
     * @param {string} containerId the HTML identifier of the container where the target plugin should be load into.
     * @param {function} objLoadedCallback callback function to be called when the object has been loaded.
     */
    loadJS(fullPath, className, jsonData, containerId, objLoadedCallback) {
        if (this.loading) {
            //Try again later
            setTimeout(function() { this.loadJS(fullPath, className, jsonData, containerId, objLoadedCallback); }.bind(this), 50);
        }
        else {
            this.loading = true; 
            var fullClassName = className + ".js";
            //If the script is not in the <head>, add it
            if (!this.resourceAlreadyExists(fullClassName)) {
                var script = document.createElement('script');
                var url = "/?path=" + fullClassName + "&TextMode=1";
                script.type = 'text/javascript';
                script.src = url;
                var jsLoader = function() {
                    this.jsLoaded(className, fullPath, jsonData, containerId, objLoadedCallback);
                    this.loading = false; 
                }.bind(this);
                var jsLoaderFailed = function() {
                    this.loading = false; 
                }.bind(this);
                script.onreadystatechange = jsLoader;
                script.onload = jsLoader;
                script.onerror = jsLoaderFailed;
                this.head.appendChild(script);
            }
            //Otherwise just load it
            else {
                this.jsLoaded(className, fullPath, jsonData, containerId, objLoadedCallback);
                this.loading = false; 
            }
        }
    }

    /**
     * Loads the css associated to the className.
     *
     * @param {string} className the name of the class (which shall inherit from MARTeObject) (e.g. HttpObjectBrowser).
     */
    loadCSS(className) {
        var fullClassName = className + ".css";
        //If the css is not in the <head>, add it
        if (!this.resourceAlreadyExists(fullClassName)) {
            var url = "/?path=" + fullClassName + "&TextMode=1";
            //But first check that the URL is valid
            var link = document.createElement("link");
            link.type = "text/css";
            link.rel = "stylesheet";
            link.href = url;
            this.head.appendChild(link);
        }
    }

    /**
     * Tries to create a new instance of the className (which inherits from the MARTeObject) and if successful call prepareDisplay and displayData on the instance.
     * 
     * @param {string} className the name of the class (e.g. HttpObjectBrowser).
     * @param {string} fullPath the object path (e.g. A/B/C).
     * @param {obj} jsonData json data as received from the server and that should be offered to the plugin (see MARTeObject::displayData).
     * @param {string} containerId the HTML identifier of the container where the target plugin should be load into.
     * @param {function} objLoadedCallback callback function to be called when the object has been loaded.
     */
    jsLoaded(className, fullPath, jsonData, containerId, objLoadedCallback) {
        var objExists = eval("typeof(" + className + ");");
        if (objExists !== "undefined") {
            var obj = eval("new " + className + "();");
            var htmlElem = document.getElementById(containerId);
            obj.setPath(fullPath);
            htmlElem.innerHTML = "";
            obj.prepareDisplay(htmlElem);
            obj.displayData(jsonData);
            if (objLoadedCallback !== undefined) {
                objLoadedCallback(obj);
            }
        }
        else {
            console.log("[" + className + "] does not exist");
        }
    }
}
