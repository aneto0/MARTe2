class MARTeObject {
    constructor() {
    }

    /**
     * TODO
     */
    displayData(target, jsonData) {
        var str = JSON.stringify(jsonData, null, 2);
        target.innerHTML = str;
    }
}

