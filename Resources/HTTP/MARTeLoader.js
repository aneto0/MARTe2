/**
 * @file MARTeLoader.js 
 * @brief Source file for class MARTeLoader.js
 * @date 27/03/2019
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
 * the class MARTeLoader (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/**
 * @brief Helper class that allow to load javascript and css code based on a given class name.
 */
class MARTeLoader {

    /**
     * @brief Singleton access to the Loader class.
     * @return the singleton of the MARTeLoader.
     */
    static instance() {
        if (this.ml == undefined) {
            this.ml = new MARTeLoader();
            this.ml.urlCache = {};
            this.ml.checkingIffUrlExist = false;
            this.ml.head = document.getElementsByTagName('head')[0];
        }
        return this.ml;
    }

    /**
     * @brief Only executes the callback if the url exists in the server. 
     * @param[in] url the url to be checked.
     * @param[out] callbackOK the function callback to be called if the resource exists in the server.
     * @param[out] callbackFailed the function callback to be called if the resource does NOT exist in the server.
     */
    executeIffUrlExists(url, callbackOK, callbackFailed) {
        if (this.checkingIffUrlExist) {
            //Try again later
            setTimeout(function() {this.executeIffUrlExists(url, callbackOK, callbackFailed); }.bind(this), 1000);
        }
        else {
            this.checkingIffUrlExist = true;
            if (this.urlCache[url] === undefined) {
                var xhttp = new XMLHttpRequest();
                var that = this;
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        callbackOK();
                        that.urlCache[url] = true;
                        that.checkingIffUrlExist = false;
                    }
                    else if (this.status == 400 || this.status == 404) {
                        if (callbackFailed !== undefined) {
                            callbackFailed();
                        }
                        that.urlCache[url] = false;
                        that.checkingIffUrlExist = false;
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
                this.checkingIffUrlExist = false;
            }
        }
    }

    /**
     * @brief Gets the javascript plugin url from a class name.
     * @param[in] className the class name for the plugin.
     * @return the url where the javascript plugin for that class name might exist.
     */
    getPluginJsUrl(className) {
        var url = "/?path=" + className + ".js&TextMode=1";
        return url;
    }

    /**
     * @brief Gets the CSS plugin url from a class name.
     * @param[in] className the class name for the plugin.
     * @return the url where the CSS plugin for that class name might exist.
     */
    getPlugiCssUrl(className) {
        var url = "/?path=" + className + ".css&TextMode=1";
        return url;
    }

    /**
     * @brief Reconstructs the URL associated to a given object path.
     * @param[in] fullPath the object path (e.g. A/B/C)
     * @return the URL associated to the object path, e.g. http://localhost:9094/A/B/C?Param1=1&TextMode=1
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
     * @brief Discovers and loads the css and javascript associated to a given class name (that inherits from MARTeObject).
     * @param[in] fullPath the objeth path (e.g. A/B/C).
     * @param[in] className the name of the class (which shall inherit from MARTeObject) (e.g. HttpObjectBrowser). If the className is not specified, or has 0 length, then the class name is discovered in the member Class from the json retrieved using TextMode = 0 on the fullPath.
     * @param[in] containerId the HTML identifier of the container where the target plugin should be load into.
     */
    load(fullPath, className, containerId) {
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
                        that.executeIffUrlExists(jsUrl,
                                //If there is a javascript try to load it.
                                function() {
                                    that.loadJS(className, fullPath, jsonData, containerId);
                                }.bind(this),
                                //Otherwise just show the raw data.
                                function() {
                                    if (this.standardDisplay === undefined) {
                                        this.standardDisplay = new MARTeObject();
                                        this.standardDisplay.setPath(fullPath);
                                        this.standardDisplay.prepareDisplay(containerHtmlElem);
                                        this.standardDisplay.displayData(jsonData);
                                    }
                                }.bind(this),
                                );
                        that.executeIffUrlExists(cssUrl, function() {
                            that.loadCSS(className);
                        });
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
     * @brief Checks if the resource was already added to the <head>
     * @param[in] marteClassName the class name that identifies the resource.
     * @return true if the resource already exists.
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
     * @brief Loads the javascript associated to the className and if successful updates the target HTML container.
     * @param[in] className the name of the class (e.g. HttpObjectBrowser).
     * @param[in] fullPath the objeth path (e.g. A/B/C).
     * @param[in] jsonData json data as received from the server and that should be offered to the plugin (see jsLoaded).
     * @param[in] containerId the HTML identifier of the container where the target plugin should be load into.
     */
    loadJS(className, fullPath, jsonData, containerId) {
        var fullClassName = className + ".js";
        //If the script is not in the <head>, add it
        if (!this.resourceAlreadyExists(fullClassName)) {
            var script = document.createElement('script');
            var url = "/?path=" + fullClassName + "&TextMode=1";
            script.type = 'text/javascript';
            script.src = url;
            var loaded = false;
            var jsLoader = function() {
                if (!loaded) {
                    loaded = true;
                    this.jsLoaded(className, fullPath, jsonData, containerId);
                }
            }.bind(this);
            script.onreadystatechange = jsLoader;
            script.onload = jsLoader;
            this.head.appendChild(script);
        }
        //Otherwise just load it
        else {
            this.jsLoaded(className, fullPath, jsonData, containerId);
        }
    }

    /**
     * @brief Loads the css associated to the className.
     * @param[in] className the name of the class (which shall inherit from MARTeObject) (e.g. HttpObjectBrowser).
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
     * @brief Tries to create a new instance of the className (which inherits from the MARTeObject) and if successful call prepareDisplay and displayData on the instance.
     * @param[in] className the name of the class (e.g. HttpObjectBrowser).
     * @param[in] fullPath the objeth path (e.g. A/B/C).
     * @param[in] jsonData json data as received from the server and that should be offered to the plugin (see MARTeObject::displayData).
     * @param[in] containerId the HTML identifier of the container where the target plugin should be load into.
     */
    jsLoaded(className, fullPath, jsonData, containerId) {
        var objExists = eval("typeof(" + className + ");");
        if (objExists !== "undefined") {
            var obj = eval("new " + className + "();");
            var htmlElem = document.getElementById(containerId);
            obj.setPath(fullPath);
            obj.prepareDisplay(htmlElem);
            obj.displayData(jsonData);
        }
    }

}
