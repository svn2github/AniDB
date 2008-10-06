/* file Animations
 * author fahrenheit (alka.setzer@gmail.com)
 * version 1.0 (25.07.2007)
 */

// GLOBALs //

var globalStatus = new statusInformation(); // Global status information

/* Removes rows from a given TBody */
function clearTableRows(tbody) { while (tbody.rows.length) tbody.removeChild(tbody.rows[0]); }

/* Auxiliar function that creates li elements */
function createLi(parentNode, text, className, onclick) {
	var li = document.createElement('li');
	if (className) li.className = className;
	if (text) li.appendChild(document.createTextNode(text));
	if (onclick) li.onclick = onclick;
	if (parentNode) parentNode.appendChild(li);
	else return li;
}

/* Function that creates tabs */
function createTabs(parentNode, optionArray, className) {
	var tabdiv = document.createElement('div');
	tabdiv.className = 'horizontalOptions';
	if (optionArray && optionArray.length) {
		var ul = document.createElement('ul');
		if (className) ul.className = className;
		for (var i = 0; i < optionArray.length; i++) {
			var option = optionArray[i];
			createLi(ul,option['text'],option['className']+(option['selected'] && option['selected'] == true ? ' selected' : ''),option['onclick']);
		}
		tabdiv.appendChild(ul);
	}
	if (parentNode) parentNode.appendChild(tabdiv);
	else return(tabdiv);
}

/* statusInformation Object */
function statusInformation() {
	this.container = null;
	this.text = '';
	this.loadingbar_color = 'green';
	this.loadingbar_initial = -120;
	this.loadingbar_imageWidth = 240;
	this.loadingbar_eachPercent = (this.loadingbar_imageWidth/2)/100;
	this.loadingbar_percentage = 0;
	this.loadingbar_tooltip = '';
	this.barElement = null;
	this.textElement = null;
	this.brElement = document.createElement('br');
	this.clearContainer = function() {
		this.container.nodeValue = '';
		while (this.container.childNodes.length) this.container.removeChild(this.container.firstChild);
		this.barElement = null;
		this.textElement = null;
		this.brElement = null;
	}
	this.init = function() {
		if (!this.container) return;
		this.clearContainer();
		this.container.appendChild(this.createText());
		this.container.appendChild(this.createBr());
		this.container.appendChild(this.createBar());
	}
	this.createText = function() {
		var span = document.createElement('span');
		span.style.dispaly = 'none';
		span.style.align = 'center';
		span.appendChild(document.createTextNode(' '));
		this.textElement = span;
		return span;
	}
	this.createBar = function() {
		var div = document.createElement('div');
		div.className = 'loadingbar loadingbar_'+this.loadingbar_color;
		div.style.display = 'none';
		this.barElement = div;
		return div;
	}
	this.createBr = function() { 
		this.brElement = document.createElement('br'); 
		this.brElement.style.display = 'none';
		return this.brElement;
	}
	this.updateText = function(text, doAppend) {
		if (!this.textElement) this.init();
		this.text = text;
		if (!doAppend) this.textElement.firstChild.nodeValue = text;
		else this.textElement.firstChild.nodeValue += text;
		this.textElement.style.display = '';
		this.brElement.style.display = 'none';
		this.barElement.style.display = 'none';
	}
	this.updateBar = function(percentage, tooltip) {
		if (!this.barElement) this.init();
		this.loadingbar_tooltip = tooltip;
		this.loadingbar_percentage = percentage;
		var percentageWidth = this.loadingbar_eachPercent * percentage;
		var newProgress = (this.loadingbar_initial+percentageWidth)+'px';
		this.barElement.style.backgroundPosition = newProgress+' 0';
		this.barElement.title = tooltip + percentage + '%';
		this.textElement.style.display = 'none';
		this.brElement.style.display = 'none';
		this.barElement.style.display = '';
	}
	this.updateBarWithText = function(text, percentage, tooltip) {
		this.updateText(text);
		this.updateBar(percentage,tooltip);
		this.textElement.style.display = '';
		this.brElement.style.display = '';
		this.barElement.style.display = '';
	}
	this.clearAfterTimeout = function(obj,timeout) {
		setTimeout(obj+".clearContainer()", timeout);
	}
}

animate = {
	elements: new Array(),
	initElement: function (element) {
		if (typeof element == 'string')
			element = document.getElementById(element);
		element.fadeIn = animate.fadeIn;
		element.fadeOut = animate.fadeOut;
		element.slideDown = animate.slideDown;
		element.slideUp = animate.slideUp;
		if (animate.elements.indexOf(element) < 0) animate.elements.push(element);
	},
	releaseElement: function (element) {
		element.fadeIn = null;
		element.fadeOut = null;
		element.slideUp = null;
		element.slideDown = null;
		animate.elements.splice(animate.elements.indexOf(element),1);
	},
	setOpacitiy: function(elementID,value,func,speed) {
		if (!elementID) return;
		var element = animate.elements[elementID];
		if (!element) return;
		if (func == 'fadeIn' && value == 0 && (element.style.display == 'none' || element.style.visibility == 'hidden')) {
			element.style.display = '';
			element.style.visibility = '';
		}
		var val = (func == 'fadeIn' ? value : (speed-value))/speed;
		element.style.opacity = val;
		element.style.filter = 'alpha(opacity=' + val*100 + ')';
	},
	doFade: function(element, func, speed) {
		if (!speed) speed = 10;
		else if (speed < 0) speed = 0;
		else if (speed > 100) speed = 100;
		for (var i = 0; i < speed+1; i++)
			setTimeout('animate.setOpacitiy("'+animate.elements.indexOf(element)+'","'+i+'","'+func+'","'+speed+'")',100*i);
	},
	setHeight: function(elementID, value, func, speed, step) {
		if (!elementID) return;
		var element = animate.elements[elementID];
		if (!element) return;
		if (func == 'slideDown' && value == 0 && (element.style.display == 'none' || element.style.visibility == 'hidden')) {
			element.style.display = '';
			element.style.visibility = '';
		}
		var val = (func == 'slideDown' ? value * step : (speed-value))/speed;
		alert('func: '+func+'\nvalue: '+value+'\nspeed: '+speed+'\nstep: '+step+'\nelement.height: '+element.style.height);
		element.style.height = val+'px';
		element.style.overflow = 'hidden';
	},
	doSlide: function(element, func, speed, limit) {
		if (!speed) speed = 10;
		else if (speed < 0) speed = 0;
		else if (speed > 100) speed = 100;
		var height = element.clientHeight;
		alert('height: '+height);
		var step = height / speed;
		for (var i = 0; i < speed+1; i++)
			setTimeout('animate.setHeight("'+animate.elements.indexOf(element)+'","'+i+'","'+func+'","'+speed+'","'+step+'")',100*i);
	},
	fadeIn: function(speed) { animate.doFade(this,'fadeIn',speed); },
	fadeOut: function(speed) { animate.doFade(this,'fadeOut',speed); },
	slideUp: function(speed) { animate.doSlide(this,'slideUp',speed); },
	slideDown: function(speed) { animate.doSlide(this,'slideDown',speed); }
}

dragDrop = {
	keySpeed: 10, // pixels per keypress event
	initialMouseX: undefined,
	initialMouseY: undefined,
	startX: undefined,
	startY: undefined,
	dXKeys: undefined,
	dYKeys: undefined,
	draggedObject: undefined,
	initElement: function (element, text, smallText, buttons) {
		if (typeof element == 'string')
			element = document.getElementById(element);
		// copy everything inside a very nice content div
		var contentDiv = document.createElement('div');
		contentDiv.className = 'content';
		while(element.childNodes.length) 
			contentDiv.appendChild(element.childNodes[0]);
		element.appendChild(contentDiv);
		// header
		var h3 = document.createElement('h3');
		h3.title = 'Click to fold/unfold';
		h3.ondblclick = function() {
			var content = getElementsByClassName(this.parentNode.getElementsByTagName('div'), 'content', true)[0];
			var buttons = getElementsByClassName(this.parentNode.getElementsByTagName('div'), 'buttons', true)[0];
			if (!content) return;
			var cHeight = 2 + content.clientHeight;
			if (buttons) cHeight += buttons.clientHeight;
			var num = 0;
			if (content.style.visibility != 'hidden') num = Number(this.parentNode.clientHeight - cHeight);
			else num = Number(this.parentNode.clientHeight + cHeight);
			this.parentNode.style.height = num+'px';
			this.parentNode.style.overflow = 'hidden';
			content.style.visibility = (content.style.visibility == 'hidden' ? 'visible' : 'hidden');
		}
		if (text) h3.appendChild(document.createTextNode(text));
		if (smallText) {
			var small = document.createElement('small');
			small.appendChild(document.createTextNode(smallText));
			h3.appendChild(small);
		}
		h3.onmousedown = dragDrop.startDragMouse;
		element.insertBefore(h3,element.firstChild);
		// anchor
		var a = createIcon(null, '[x]', 'removeme', function() { this.parentNode.style.display = 'none'; }, 'Close', 'i_close');
		element.insertBefore(a,element.firstChild);
		// buttons
		if (buttons && buttons.length) {
			var buttonLayer = document.createElement('div');
			buttonLayer.className = 'buttons';
			for (var i = 0; i < buttons.length; i++) 
				buttonLayer.appendChild(buttons[i]);
			element.appendChild(buttonLayer);
		}
		/*
		var links = element.getElementsByTagName('a');
		var lastLink = links[links.length-1];
		lastLink.relatedElement = element;
		lastLink.onclick = dragDrop.startDragKeys; */
	},
	startDragMouse: function (e) {
		dragDrop.startDrag(this);
		var evt = e || window.event;
		dragDrop.initialMouseX = evt.clientX;
		dragDrop.initialMouseY = evt.clientY;
		addEventSimple(document,'mousemove',dragDrop.dragMouse);
		addEventSimple(document,'mouseup',dragDrop.releaseElement);
		return false;
	},
	startDragKeys: function () {
		dragDrop.startDrag(this.relatedElement);
		dragDrop.dXKeys = dragDrop.dYKeys = 0;
		addEventSimple(document,'keydown',dragDrop.dragKeys);
		addEventSimple(document,'keypress',dragDrop.switchKeyEvents);
		this.blur();
		return false;
	},
	startDrag: function (obj) {
		if (dragDrop.draggedObject)
			dragDrop.releaseElement();
		obj = obj.parentNode;
		dragDrop.startX = obj.offsetLeft;
		dragDrop.startY = obj.offsetTop;
		dragDrop.draggedObject = obj;
		obj.className += ' dragged';
	},
	dragMouse: function (e) {
		var evt = e || window.event;
		var dX = evt.clientX - dragDrop.initialMouseX;
		var dY = evt.clientY - dragDrop.initialMouseY;
		dragDrop.setPosition(dX,dY);
		return false;
	},
	dragKeys: function(e) {
		var evt = e || window.event;
		var key = evt.keyCode;
		switch (key) {
			case 37:	// left
			case 63234:
				dragDrop.dXKeys -= dragDrop.keySpeed;
				break;
			case 38:	// up
			case 63232:
				dragDrop.dYKeys -= dragDrop.keySpeed;
				break;
			case 39:	// right
			case 63235:
				dragDrop.dXKeys += dragDrop.keySpeed;
				break;
			case 40:	// down
			case 63233:
				dragDrop.dYKeys += dragDrop.keySpeed;
				break;
			case 13: 	// enter
			case 27: 	// escape
				dragDrop.releaseElement();
				return false;
			default:
				return true;
		}
		dragDrop.setPosition(dragDrop.dXKeys,dragDrop.dYKeys);
		if (evt.preventDefault)
			evt.preventDefault();
		return false;
	},
	setPosition: function (dx,dy) {
		dragDrop.draggedObject.style.left = dragDrop.startX + dx + 'px';
		dragDrop.draggedObject.style.top = dragDrop.startY + dy + 'px';
	},
	switchKeyEvents: function () {
		// for Opera and Safari 1.3
		removeEventSimple(document,'keydown',dragDrop.dragKeys);
		removeEventSimple(document,'keypress',dragDrop.switchKeyEvents);
		addEventSimple(document,'keypress',dragDrop.dragKeys);
	},
	releaseElement: function() {
		removeEventSimple(document,'mousemove',dragDrop.dragMouse);
		removeEventSimple(document,'mouseup',dragDrop.releaseElement);
		removeEventSimple(document,'keypress',dragDrop.dragKeys);
		removeEventSimple(document,'keypress',dragDrop.switchKeyEvents);
		removeEventSimple(document,'keydown',dragDrop.dragKeys);
		dragDrop.draggedObject.className = dragDrop.draggedObject.className.replace(/dragged/,'');
		dragDrop.draggedObject = null;
	}
}

