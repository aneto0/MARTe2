class MARTeObject {
    constructor() {
    }

    /**
     * @brief Virtual display method to be specialised by the inherited objects in order to create the display environment.
     * @details The default implementation creates a text area to show the object value in a json structure.
     * @param[in] target the target container where to display the data.
     */
    prepareDisplay(target) {
        this.textarea = document.createElement('textarea');
        this.inputRefresh = document.createElement('input');
        this.inputRefresh.value = "1000";
		this.textarea.setAttribute("disabled", true);
		var textRefreshRate = document.createTextNode("Refresh rate (ms)");
		target.innerHTML = "";
		target.appendChild(textRefreshRate);
		target.appendChild(this.inputRefresh);
		target.appendChild(this.textarea);
		
		var refreshInt = parseInt(this.inputRefresh.value); 
		if (!isNaN(refreshInt)) {
			this.refresh(refreshInt);
		}
    }

    /**
     * @brief Virtual display method to be specialised by the inherited objects in order to update the value of components.
     * @details The default implementation shows the object value in a json structure.
     * @param[in] jsonData the object information in json format.
     */
    displayData(jsonData) {
		this.textarea.innerHTML = JSON.stringify(jsonData, null, '\t');
    }
    
    /**
     * @brief Sets the object path.
     * @param[in] the object path.
     */
    setPath(path) {
    	this.path = path;
    }
    
    /**
     * @brief Gets the object path.
     * @return the object path.
     */
    getPath() {
    	return this.path;
    }
    
    /**
     * @brief Refreshes the current object with new data from the server.
     * @param[in] period optional refresh period in ms.
     */
    refresh(period) {
    	if (period === undefined) {
    		period = 0;
    	}
    	//Get the URL and add all the extra parameters
		var fullURL = MARTeLoader.instance().getDataUrl(this.getPath());
    	var xhttp = new XMLHttpRequest();
		var that = this;
		xhttp.onreadystatechange = function() {
			if (this.readyState == 4 && this.status == 200) {
				try {
					var jsonData = JSON.parse(this.responseText);
					that.displayData(jsonData);
					if (period > 0) {
						setTimeout(that.refresh(period), period);
					}
				}
				catch (e) {
					console.log(e);
				}

			}
		};		
		xhttp.open("GET", fullURL, true);
		xhttp.send();
    }
}

