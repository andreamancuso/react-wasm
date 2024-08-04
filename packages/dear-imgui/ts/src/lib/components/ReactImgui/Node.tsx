import { FunctionComponent } from "react";
import { NodeProps, PropsWithChildren } from "./types";

export const Node: FunctionComponent<PropsWithChildren & NodeProps> = ({
    root,
    children,
    style,
}) => {
    return (
        <node root={root} style={style}>
            {children}
        </node>
    );
};
