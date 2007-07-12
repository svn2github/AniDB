String.exists = function(str, item) {
    return (str.indexOf(item) != -1)
}

var equality = function(item) {
    return function(other) {
        return item == other
    }
}

var CollectionNameSpace = {
    forEach: function(ary, operation) {
        for (var i = 0; i < ary.length; i++)
        {
            operation(ary[i])
        }
    },

    exists: function(ary, condition) {
        return (CollectionNameSpace.find(ary, condition) != null)
    },

    find: function(ary, condition) {
        for (var i = 0; i < ary.length; i++)
            if (condition(ary[i])) return ary[i]
        return null
    },

    findall: function(ary, condition) {
        var foundItems = new Array();
        CollectionNameSpace.forEach(ary, function collect(item) {
            if (condition(item)) foundItems.push(item);
        })
        return foundItems;
    }
}

Function.prototype.bind = function(object) {
    var method = this;
    return function() {
        method.apply(object, arguments);
    }
}

function inherits(newClass, baseClass) {
    newClass.prototype = new baseClass()
    newClass.prototype.base = baseClass.prototype
}


