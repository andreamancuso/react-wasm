const hello = () => {
    postMessage({ message: "hello" });
};

addEventListener("message", (evt) => {
    const canvas = evt.data.canvas;
    // const gl = canvas.getContext("webgl");

    console.log(evt.data);
});
