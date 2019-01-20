class HttpObjectBrowser extends MARTeObject {
	constructor() {
		super();
	}


    /**
     * TODO
     */
    prepareDisplay(target) {
        target.innerHTML = "";
        this.target = target;
    }

    /**
     * TODO
     */
	displayData(jsonData) {		
		this.createTreeNodes(jsonData, "", this.target);
		this.registerTreeLinks(jsonData, "");
	}

	/**
	 * TODO
	 */
	isNodeOpen(btn) {
		return (btn.innerHTML === "-");
	}

	/**
	 * TODO
	 */
	isNodePopulated(node) {
		//two buttons
		return (node.children.length > 2);
	}

	/**
	 * TODO
	 */
	expandTreeNode(path, btn, obj) {
		if (!this.isNodeOpen(btn)) {
			if (!this.isNodePopulated(obj)) {
				var xhttp = new XMLHttpRequest();
				var that = this;
				xhttp.onreadystatechange = function() {
					if (this.readyState == 4 && this.status == 200) {
						var jsonData = JSON.parse(this.responseText);
						that.createTreeNodes(jsonData, path, obj);
						that.registerTreeLinks(jsonData, path);
					}
					else {
						//TODO
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
				var ul = obj.querySelector(".nested");
				if (ul !== null) {
					ul.classList.add("active");
					btn.innerHTML = "-";
				}
			}
		}
		else {
			var ul = obj.querySelector(".nested");
			if (ul !== null) {
				ul.classList.remove("active");
				btn.innerHTML = "+";
			}
			btn.innerHTML = "+";
		}
	}

	/**
	 * TODO
	 */
	showTreeNode(path, obj, rightPaneContainerId) {
		var marteClassName = obj.getAttribute("marteClassName");
		if (marteClassName !== null) {
			MARTeLoader.instance().load(path, marteClassName, rightPaneContainerId);
		}		
	}

	/**
	 * TODO
	 */
	generateId(id) {
		return id.replace(/\//g, '_');
	}

	/**
	 * TODO
	 */
	createTreeNodes(jsonData, fullpath, parent) {
		var i = 0;
		var panelConfig = MARTeLoader.instance().getPanelConfig();
		var nodeName = "" + i;
		var hasMoreNodes = (jsonData[nodeName] !== undefined);
		if (fullpath.length > 0) {
			if (!fullpath.endsWith("/")) {
				fullpath += "/";
			}
		}
		var ul = document.createElement("ul");
		parent.appendChild(ul);
		ul.setAttribute("class", "nested active");
		parent = ul;
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
			var showObjectBtnTxt = document.createTextNode(">");
			var showObjectBtnId = "sbtn_" + pathid;
			showObjectBtn.setAttribute("id", showObjectBtnId);
			showObjectBtn.setAttribute("class", "dropbtn");
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
					var rightPaneContainerId = MARTeLoader.instance().getRightPaneContainerId(r, c);
					var divContainerLinkId = showObjectBtnId + rightPaneContainerId;					
					divContainerLink.setAttribute("id", divContainerLinkId);
					var divContainerLinkTxt = document.createTextNode(r + "x" + c);
					divContainerLink.appendChild(divContainerLinkTxt);
					dropdownDivContainers.appendChild(divContainerLink);
				}
			}
			divParentContainer.appendChild(dropdownDivContainers);								
			li.appendChild(divParentContainer);
			parent.appendChild(li);

			i++;
			nodeName = "" + i;
			hasMoreNodes = (jsonData[nodeName] !== undefined);
		}
	}

	/**
	 * TODO
	 */
	registerTreeLinks(jsonData, fullpath) {
		var i = 0;
		var nodeName = "" + i;
		var panelConfig = MARTeLoader.instance().getPanelConfig();
		
		var hasMoreNodes = (jsonData[nodeName] !== undefined);
		if (fullpath.length > 0) {
			if (!fullpath.endsWith("/")) {
				fullpath += "/";
			}
		}
		while (hasMoreNodes) {
			var child = jsonData[nodeName];
			var objName = child["Name"];
			var objpath = fullpath + objName;
			var pathid = this.generateId(objpath);
			var expandObjectBtnId = "ebtn_" + pathid;
			var liid = "li_" + pathid;
			var li = document.getElementById(liid);
			var expandObjectBtn = document.getElementById(expandObjectBtnId);
			if (expandObjectBtn !== null) {				
				expandObjectBtn.addEventListener("click",
					function(ev, objpath, expandObjectBtn, li) {
						console.log(objpath);
						console.log(li);
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
					var rightPaneContainerId = MARTeLoader.instance().getRightPaneContainerId(r, c);
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

			i++;
			nodeName = "" + i;
			hasMoreNodes = (jsonData[nodeName] !== undefined);
		}
	}
}

