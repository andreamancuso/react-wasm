const nodeImgui = require("./node-imgui");

nodeImgui.init();

const rootNode = {
  id: 0,
  type: "node",
  root: true,
  style: {
    height: "100%",
  },
};

const contentNode = {
  id: 1,
  type: "node",
  style: {
    width: "50%",
    height: "100%",
  },
};

const inputText = {
  id: 2,
  type: "input-text",
};

setTimeout(() => {
  nodeImgui.setElement(JSON.stringify(rootNode));
  nodeImgui.setElement(JSON.stringify(contentNode));
  nodeImgui.setElement(JSON.stringify(inputText));
  nodeImgui.setChildren(1, "[2]");
  nodeImgui.setChildren(0, "[1]");
  // nodeImgui.showDebugWindow();
}, 500);

let flag = true;
(function keepProcessRunning() {
  setTimeout(() => flag && keepProcessRunning(), 1000);
})();
