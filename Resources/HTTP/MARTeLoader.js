class MARTeLoader {

	static instance() {
		if (this.ml == undefined) {
			this.ml = new MARTeLoader();
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
		var xhttp = new XMLHttpRequest();
		xhttp.onreadystatechange = function() {
			if (this.readyState == 4 && this.status == 200) {
				callbackOK();
			}
			else if (this.status == 400 || this.status == 404) {
				if (callbackFailed !== undefined) {
					callbackFailed();
				}
			}
		};
		xhttp.open("HEAD", url, true);
		xhttp.send();
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
	 * @brief TODO
	 */
	getDataUrl(fullPath) {
		//Get the URL and add all the extra parameters
		var pname = window.location.pathname;
		var getparams = window.location.search;
		getparams = getparams.replace("TextMode=1", "TextMode=0");
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

	load(fullPath, className, containerId) {
		var xhttp = new XMLHttpRequest();
		var that = this;
		xhttp.onreadystatechange = function() {
			if (this.readyState == 4 && this.status == 200) {
				var containerHtmlElem = document.getElementById(containerId);
				var jsonStruct;
				try {
					jsonStruct = JSON.parse(this.responseText);
					if (className.length === 0) {
						className = jsonStruct["Class"];
					}
					if (className !== undefined) {
						//Check if the url for the plugin exists
						var jsUrl = that.getPluginJsUrl(className);
						var cssUrl = that.getPlugiCssUrl(className);
						that.executeIffUrlExists(jsUrl,
							//If there is a javascript try to load it.
							function() {
								that.loadJS(className, jsonStruct, containerId);
							}.bind(this),
							//Otherwise just show the raw data.
							function() {
								var textarea = document.createElement('textarea');
								textarea.setAttribute("disabled", true);
								textarea.innerHTML = JSON.stringify(jsonStruct, null, '\t');
								containerHtmlElem.innerHTML = "";
								containerHtmlElem.appendChild(textarea);
							}.bind(this),
						);
						that.executeIffUrlExists(cssUrl, function() {
							that.loadCSS(className);
						});
					}
					else {
						//TODO
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
			else {
				//TODO
			}
		};
		//Get the URL and add all the extra parameters
		var fullURL = this.getDataUrl(fullPath);
		xhttp.open("GET", fullURL, true);
		xhttp.send();
	}

	loadJS(className, jsonStruct, containerId) {
		var head = document.getElementsByTagName('head')[0];
		//Check if the script already exists
		var scriptAlreadyExists = false;
		var fullClassName = className + ".js";
		var url = "/?path=" + fullClassName + "&TextMode=1";
		var nChildren = head.children.length;
		for (var c = 0; (c < nChildren) && (!scriptAlreadyExists); c++) {
			var src = head.children[c].getAttribute("src");
			if (src !== null) {
				scriptAlreadyExists = src.includes(fullClassName);
			}
		}

		//If the script is not in the <head>, add it
		if (!scriptAlreadyExists) {
			var script = document.createElement('script');
			script.type = 'text/javascript';
			script.src = url;
			var loaded = false;
			var jsLoader = function() {
				if (!loaded) {
					loaded = true;
					this.jsLoaded(className, jsonStruct, containerId);
				}
			}.bind(this);
			script.onreadystatechange = jsLoader;
			script.onload = jsLoader;
			head.appendChild(script);
		}
		//Otherwise just load it
		else {
			this.jsLoaded(className, jsonStruct, containerId);
		}
	}

	jsLoaded(className, jsonStruct, containerId) {
		var objExists = eval("typeof(" + className + ");");
		if (objExists !== "undefined") {
			var obj = eval("new " + className + "();");
			var htmlElem = document.getElementById(containerId);
			obj.displayData(htmlElem, jsonStruct);
		}
	}

	loadCSS(className) {
		var head = document.getElementsByTagName('head')[0];
		//Check if the css already exists
		var cssAlreadyExists = false;
		var fullClassName = className + ".css";
		var nChildren = head.children.length;
		for (var c = 0; (c < nChildren) && (!cssAlreadyExists); c++) {
			var src = head.children[c].getAttribute("src");
			if (src !== null) {
				cssAlreadyExists = src.includes(fullClassName);
			}
		}
		//If the css is not in the <head>, add it
		if (!cssAlreadyExists) {
			var url = "/?path=" + fullClassName + "&TextMode=1";
			//But first check that the URL is valid
			var link = document.createElement("link");
			link.type = "text/css";
			link.rel = "stylesheet";
			link.href = url;
			head.appendChild(link);
		}
	}
}
