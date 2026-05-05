/**
 * Plugin renderer for a MassSpring system.
 */
class MassSpring extends MARTeObject {

    /**
     * NOOP
     */
    constructor() {
        super();
    }


    /**
     * Creates the canvas.
     *
     * @param {obj} target the target HTML container where to display the data.
     */
    prepareDisplay(target) {
        this.canvas = document.createElement("canvas");
        this.canvas.setAttribute("width", "500");
        this.canvas.setAttribute("height", "200");
        this.ctx = this.canvas.getContext("2d");
        target.appendChild(this.canvas);
        this.refresh(100);
        this.midYPoint = 100;
        this.minXPos = 50;
        this.massWidth = 50;
        this.massHeight = 50;
        this.wallWidth = 10;
        this.scaleX = 10;
        this.scaleForce = 2;
    }

    /**
     * Updates the pendulum with the new angle position.
     *
     * @param {obj} jsonData the data as received by the server and which should contain the angle.
     */
    displayData(jsonData) {
        
        var pos = this.minXPos + parseFloat(jsonData["OutputSignals"]["Position"]) * this.scaleX;
        var force = parseFloat(jsonData["InputSignals"]["Force"]);
        //console.log(force);
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        this.drawWall();
        this.drawTable();
        this.drawSpring(pos);
        this.drawMass(pos);
        this.drawForceVector(pos, force);
    }

    drawWall() {
        this.ctx.fillStyle = "black";
        const wallHeight = 120;
        this.ctx.fillRect(0, this.midYPoint - wallHeight / 2, this.wallWidth, wallHeight);
    }

    drawTable() {
        this.ctx.fillStyle = "black";
        const tableWidth = 300;
        this.ctx.fillRect(10, this.midYPoint + this.massHeight / 2, tableWidth, this.wallWidth);
    }

    drawSpring(xMass) {
        const startX = 10;
        const endX = xMass;
        const springY = this.midYPoint;
        const coils = 8;
        const amplitude = 10;

        this.ctx.beginPath();
        this.ctx.moveTo(startX, springY);

        const length = endX - startX;
        const step = length / (coils * 2);

        let x = startX;

        for (let i = 0; i < coils * 2; i++) {
            x += step;
            const yOffset = (i % 2 === 0) ? -amplitude : amplitude;
            this.ctx.lineTo(x, springY + yOffset);
        }

        this.ctx.lineTo(endX, springY);
        this.ctx.strokeStyle = "gray";
        this.ctx.lineWidth = 1;
        this.ctx.stroke();
    }

    drawForceVector(xMass, force) {
        let  startX = xMass + this.massWidth;
        let endX = startX + force * this.scaleForce;
        const forceColor = "green"
        if (force < 0) {
            startX = xMass;
            endX = xMass + force * this.scaleForce;
        }
        //console.log(startX);
        //console.log(endX);
        this.ctx.beginPath();
        this.ctx.moveTo(startX, this.midYPoint);
        this.ctx.lineTo(endX, this.midYPoint);

        let arrowSize = force / this.scaleForce;
        if (arrowSize > 5) {
            arrowSize = 5;
        }
        this.ctx.lineTo(endX - arrowSize, this.midYPoint - arrowSize);
        this.ctx.moveTo(endX, this.midYPoint);
        this.ctx.lineTo(endX - arrowSize, this.midYPoint + arrowSize);
        this.ctx.strokeStyle = forceColor;
        this.ctx.lineWidth = 1;
        this.ctx.stroke();
        
        const text = force.toFixed(1);
        this.ctx.fillStyle = forceColor;
        this.ctx.font = "12px Arial";
        this.ctx.textAlign = "left";
        if (force < 0) {
            this.ctx.textAlign = "right";
        }
        this.ctx.textBaseline = "middle"
        this.ctx.fillText(text, endX, this.midYPoint - 12);

    }

    drawMass(xMass) {
        this.ctx.fillStyle = "red";
        this.ctx.fillRect(xMass, this.midYPoint - this.massHeight/ 2, this.massWidth, this.massHeight);
        const realPos = (xMass - this.minXPos) / this.scaleX;
        const text = realPos.toFixed(2);
        this.ctx.fillStyle = "white";
        this.ctx.font = "12px Arial";
        this.ctx.textAlign = "center";
        this.ctx.textBaseline = "middle"
        this.ctx.fillText(text, xMass + this.massWidth / 2, this.midYPoint);
    }
}

