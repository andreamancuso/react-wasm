type DataBlock = number[];

const SOP = 0x01; // ASCII for 'start of heading'
const EOP = 0x17; // ASCII for 'end of transmission block'

/**
 * Attempts to parse DataBlock instances according to the following:
 *
 * | SOP | ...data... | CSUM | EOP |
 *
 * Where:
 * - CSUM is a single byte and is defined as the sum (mod 256) of all the bytes contained in the data block
 * - data may be empty
 *
 * Assumptions:
 * - SOP is a single byte (0x01)
 * - EOP is a single byte (0x17)
 * - function receives arrays of numbers, which represent bytes (value will be between 0 and 255)
 *
 */
function receiveBlock(packet: DataBlock): void {
    if (packet.length < 3) {
        // A valid packet must have at least SOP, CSUM, and EOP
        console.log("invalid packet");
        return;
    }

    // Check if the packet starts with SOP and ends with EOP
    if (packet[0] !== SOP || packet[packet.length - 1] !== EOP) {
        console.log("invalid packet");
        return;
    }

    const checksumIndex = packet.length - 2;
    let calculatedChecksum = 0;
    // We're only interested in the bytes between SOP and CSUM, exclusive
    for (let i = 1; i < checksumIndex; i += 1) {
        calculatedChecksum += packet[i];
    }

    calculatedChecksum = calculatedChecksum % 256;

    if (calculatedChecksum !== packet[checksumIndex]) {
        console.log("invalid packet");
    } else {
        console.log("Data: ", packet.slice(1, checksumIndex));
    }
}

receiveBlock([]); // empty array
receiveBlock([0, EOP]); // no SOP
receiveBlock([SOP, 0]); // no EOP
receiveBlock([2, 0, EOP]); // invalid SOP
receiveBlock([SOP, 0, 1]); // invalid EOP
receiveBlock([SOP, 1, EOP]); // invalid checksum, empty data block
receiveBlock([SOP, 120, 84, 32, 47, 0, EOP]); // invalid checksum, with non-empty data block

receiveBlock([SOP, 0, EOP]); // valid data block, empty data block
receiveBlock([SOP, 120, 84, 32, 47, 27, EOP]); // valid data block
