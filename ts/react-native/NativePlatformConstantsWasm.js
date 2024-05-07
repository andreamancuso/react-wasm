/**
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * @flow strict
 * @format
 */

import type { TurboModule } from "./react-native/Libraries/TurboModule/RCTExport";

import * as TurboModuleRegistry from "./react-native/Libraries/TurboModule/TurboModuleRegistry";

export type PlatformConstantsWasm = {|
    isTesting: boolean,
    isDisableAnimations?: boolean,
    reactNativeVersion: {|
        major: number,
        minor: number,
        patch: number,
        prerelease: ?string,
    |},
    reactNativeWasmVersion: {|
        major: number,
        minor: number,
        patch: number,
    |},
    osVersion: number,
|};

export interface Spec extends TurboModule {
    +getConstants: () => PlatformConstantsWasm;
}

export default (TurboModuleRegistry.getEnforcing<Spec>("PlatformConstants"): Spec);
