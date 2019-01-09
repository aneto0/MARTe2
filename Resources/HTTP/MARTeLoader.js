class MARTeLoader {

	static instance() {
		if (this.ml == undefined) {
			this.ml = new MARTeLoader();
		}
		return this.ml;

	}

	load(fullpath, className, containerId) {
		var xhttp = new XMLHttpRequest();
		var that = this;
		xhttp.onreadystatechange = function() {
			if (this.readyState == 4 && this.status == 200) {
				var jsonStruct = JSON.parse(this.responseText);
				if (className.length === 0) {
					className = jsonStruct["Class"];
				}
				if (className !== undefined) {
					that.loadJS(className, jsonStruct, containerId);
					that.loadCSS(className);
				}
				else {
					//TODO
				}
			}
			else {
				//TODO
			}
		};
		//Get the URL and add all the extra parameters
		var pname = window.location.pathname;
		var getparams = window.location.search;
		getparams = getparams.replace("TextMode=1", "TextMode=0");
		var fullURL = pname;
		if (fullpath.length > 0) {
			if (!fullURL.endsWith("/")) {
				fullURL += "/";
			}
			fullURL += fullpath;
		}
		fullURL += getparams;
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
		var obj = eval("new " + className + "();");
		var htmlElem = document.getElementById(containerId);
		obj.displayData(htmlElem, jsonStruct);
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
			var link = document.createElement("link");
			var url = "/?path=" + fullClassName + "&TextMode=1";
			link.type = "text/css";
			link.rel = "stylesheet";
			link.href = url;
			head.appendChild(link);
		}
	}
}
