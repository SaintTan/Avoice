# Market Information Protocol

## Order Book Update
- Prices in cents
- Asks ordered in ascending order, bids descending (so best is 0)
- Where less than 5 asks/bids occur, remaining entries will be 0
- Can arrive out of time -> sequence numbers (strictly increasing in time) allow this to be traced


