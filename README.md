# RT1-Autotrader
Autotrader for Optiver's "Ready Trader One" competition, written for Python 3.6.10 and a linux operating environment.

# Specifications
- We will need to set up SSH public/private key for SFTP communication (https://www.readytraderone.com.au/faqs/) through which we can access the resources for the autotrader, including a sample trader and data, and logfiles for tournament performance

# Market Information

### Order Book Updates
Exchange sends *order book update* messages to autotraders on the order book for the *future* and the *ETF*. This contains:
- Price & Volume for the top 5 bid/ask prices in the market for one instruent

### Trade Ticks
Exchange sends *trade ticks* messages containing:
- Prices that trading occurred at since the last *trade ticks* message
- The volumes traded at each such price

### Order Status
Exchange sends *order status* messages to flag state changes to an order (amended, cancelled, traded)

### Position Changes
Exchange sends *position change* messages to flag:
- Changes in the position in the future and ETF (remembering the exchange auto-hedges the Autotrader's position in the ETF)

### Error Messages
Exchange sends *error* message when errors such as limit breaches or invalid orders are sent. The message describes the error and identifies where applicable the order.