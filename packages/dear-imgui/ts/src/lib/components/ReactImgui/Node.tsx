import { FunctionComponent } from "react";
import { NodeProps, PropsWithChildren } from "./types";

export const Node: FunctionComponent<PropsWithChildren & NodeProps> = ({
    root,
    children,
    cull,
    style,
    hoverStyle,
    activeStyle,
    disabledStyle,
}) => {
    return (
        <node
            root={root}
            cull={cull}
            style={style}
            hoverStyle={hoverStyle}
            activeStyle={activeStyle}
            disabledStyle={disabledStyle}
        >
            {children}
        </node>
    );
};
