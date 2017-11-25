# HearthDuster
Need some dust? Don't know what to disenchant? HearthDuster shows you what cards you can safely disenchant to keep your collection at a playable level.

## Download
The current version is 0.8. See the [release notes](ReleaseNotes.md) for latest changes.

- [MacOS](https://github.com/ifeherva/HearthDuster/releases/download/v0.8.0/HearthDuster_0_8_0.dmg)
- [Windows](https://github.com/ifeherva/HearthDuster/releases/download/v0.8.0/HearthDuster_0_8_0.zip)

## How does it work?
HearthDuster reads your card collection and recommends cards that you can disenchant based on one of the following strategies:

- **Duplicates**: cards you have way too many, like 2 from the same legendary or more than 2 from a normal card. Golden and normal ones are calculated separately.
- **Joint duplicates**: cards you have way too many including golden ones. For example two normal and one golden copy of the same card means you only need to keep one normal card in order to retain the playability of your collection. This strategy will select the non-golden cards.
- **Joint duplicates golden**: same as above but keeps the normal cards to make more dust.
- **Wild**: cards that are no longer in standard.
- **Popularity**: cards that see play rarely, based on the last 14 days of ranked data from the [HSReplay.net](hsreplay.net) database.

## Usage
1. Make sure Hearthstone is running
2. Run HearthDuster
3. Disenchant, enjoy dust :)

## System requirments
Minimum Windows 7 64-bit or macOS 10.10

## Questions?
Feel free to file an issue if you have any problem or a cool idea to make this app better.

## Support
Want to help development? Donations are always appreciated.

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=DJ2829XPVB4ZC)

## License

Copyright © Istvan Fehervari. All Rights Reserved.
