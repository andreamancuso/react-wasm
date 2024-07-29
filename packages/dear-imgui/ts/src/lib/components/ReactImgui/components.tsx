import { MainComponent } from "../ReactImgui";
import { attachSubComponents } from "../../attachSubComponents";
import { BulletText } from "./BulletText";
import { Button } from "./Button";
import { Checkbox } from "./Checkbox";
import { CollapsingHeader } from "./CollapsingHeader";
import { Combo } from "./Combo";
import { Node } from "./Node";
import { InputText } from "./InputText";
import { ItemTooltip } from "./ItemTooltip";
import { MultiSlider } from "./MultiSlider";
import { Separator } from "./Separator";
import { SeparatorText } from "./SeparatorText";
import { Slider } from "./Slider";
import { TabBar } from "./TabBar";
import { TabItem } from "./TabItem";
import { DisabledText } from "./DisabledText";
import { TextWrap } from "./TextWrap";
import { TreeNode } from "./TreeNode";
import { UnformattedText } from "./UnformattedText";
import { MapView } from "./MapView";
import { Unknown } from "./Unknown";
import { Table } from "./Table";
import { Child } from "./Child";
import { Group } from "./Group";
import { DIWindow } from "./DIWindow";
import { ClippedMultiLineTextRenderer } from "./ClippedMultiLineTextRenderer";

export const components = {
    UnformattedText,
    Unknown,
    InputText,
    CollapsingHeader,
    TreeNode,
    SeparatorText,
    BulletText,
    DisabledText,
    ItemTooltip,
    TextWrap,
    Combo,
    Slider,
    MultiSlider,
    Checkbox,
    Button,
    Separator,
    DIWindow,
    Child,
    Group,
    TabBar,
    TabItem,
    Table,
    ClippedMultiLineTextRenderer,
    MapView,
    Node,
} as const;

export const ReactImgui = attachSubComponents("ReactImgui", MainComponent, components);
