const nodeImgui = require("./node-imgui");

nodeImgui.Test();

let flag = true;
(function keepProcessRunning() {
    setTimeout(() => flag && keepProcessRunning(), 1000);
})();