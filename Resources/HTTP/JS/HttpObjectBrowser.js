class HttpObjectBrowser extends MARTeObject {
	constructor() {
		super();
	}

    /**
     * TODO
     */
	displayData(target, jsonData) {
		var htmlTxt = "<ud id=\"HttpObjectBrowserTree\">\n";
        htmlTxt += this.createTree(jsonData);
		htmlTxt += "</ud>\n";
		target.innerHTML = htmlTxt;
	}
	
	/**
	 * TODO
	 */
	createTree(jsonData) {
        var str = "";
		var i = 0;
		var nodeName = "" + i;
		var hasMoreNodes = (jsonData[nodeName] !== undefined);
        if (hasMoreNodes) {
            var txt = jsonData["Name"];
            txt += " (" + jsonData["Class"] + ")";
            str += "<li><span class=\"caret\">" + txt + "</span>\n";
            str += "<ul class=\"nested\">\n";
        }
        else {
            var txt = jsonData["Name"];
            txt += " (" + jsonData["Class"] + ")";
            str += "<li>" + txt + "</li>\n";
        }
        console.log("jsonData[" + nodeName + "]");
        var hadNodes = hasMoreNodes;
		while (hasMoreNodes) {
            str += this.createTree(jsonData[nodeName], str); 
            i++;
            nodeName = "" + i;
            hasMoreNodes = (jsonData[nodeName] !== undefined);
		}
        if (hadNodes) {
            str += "</ul>\n";
            str += "</li>\n";
        }
        return str;
	} 
}

