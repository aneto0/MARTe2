class MARTeLoader {

    static instance () {
        if(this.ml == undefined) {
            this.ml = new MARTeLoader();
        }
        return this.ml;
        
    }

    load() {
        var xhttp = new XMLHttpRequest();
        var that = this;
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                console.log(this.responseText);
                var jsonStruct = JSON.parse(this.responseText);
                var className = jsonStruct["Class"];
                if (className !== undefined) {
                    that.loadJS(className, jsonStruct);
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
        xhttp.open("GET", pname + getparams, true);
        xhttp.send();
    }

    loadJS(className, jsonStruct) {
        //TODO, check if the script already exists
        var head = document.getElementsByTagName('head')[0];
        var script = document.createElement('script');
        var fullClassName = className + ".js";
        var url = "/?path=" + fullClassName + "&TextMode=1";
        script.type = 'text/javascript';
        script.src = url;
        var loaded = false;
        var jsLoader = function() {
            if (!loaded) {
                loaded = true;
                alert(this.jsLoaded);
                this.jsLoaded(className, jsonStruct);
            }
        }.bind(this);
        script.onreadystatechange = jsLoader;
        script.onload = jsLoader;
        head.appendChild(script);
    }

    jsLoaded(className, jsonStruct) {
        var obj = eval("new " + className + "();");
        var htmlElem = document.getElementById("container");
        obj.displayData(htmlElem, jsonStruct);
        //alert(obj);
    }
}

