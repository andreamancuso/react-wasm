import React, { useRef } from 'react';
import { ReactImgui } from '@react-wasm/dear-imgui';

import "./App.css";

function App() {
  const containerRef = useRef<HTMLDivElement>(null);
  
  return (
    <div id="app" ref={containerRef}>
      <ReactImgui containerRef={containerRef}>
        <ReactImgui.CollapsingHeader label="Help">
          <ReactImgui.SeparatorText label="ABOUT THIS DEMO:" />
          <ReactImgui.BulletText text="Sections below are demonstrating many aspects of the library." />
          <ReactImgui.BulletText text='The "Examples" menu above leads to more demo contents.' />
          <ReactImgui.BulletText
              text={`The "Tools" menu above gives access to: About Box, Style Editor,\nand Metrics/Debugger (general purpose Dear ImGui debugging tool).`}
          />
          <ReactImgui.SeparatorText label="PROGRAMMER GUIDE:" />
          <ReactImgui.BulletText text="See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!" />
          <ReactImgui.BulletText text="See comments in imgui.cpp." />
          <ReactImgui.BulletText text="See example applications in the examples/ folder." />
          <ReactImgui.BulletText text="Read the FAQ at https://www.dearimgui.com/faq/" />
          <ReactImgui.BulletText text="Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls." />
          <ReactImgui.BulletText text="Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls." />

          <ReactImgui.SeparatorText label="USER GUIDE:" />
        </ReactImgui.CollapsingHeader>
      </ReactImgui>
    </div>
  );
}

export default App;
