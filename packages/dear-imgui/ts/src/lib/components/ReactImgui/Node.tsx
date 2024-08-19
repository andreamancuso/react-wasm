import { FunctionComponent } from "react";
import { NodeProps, PropsWithChildren } from "./types";

export const Node: FunctionComponent<PropsWithChildren & NodeProps> = ({
    root,
    children,
    style,
    cull,
}) => {
    return (
        <node root={root} cull={cull} style={style}>
            {children}
        </node>
    );
};
