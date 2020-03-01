"""
An estimated structure of the BaseAutoTrader interface.
"""

import logging
import asyncio
import enum
from typing import List, Tuple

class Side(enum.Enum):
    """Enumerable for side of offers."""
    BUY = 0
    SELL = 1

class Lifespan(enum.Enum):
    """Enumerable for lifespan of market offers."""
    FILL_AND_KILL = 0
    GOOD_FOR_DAY = 1

class Instrument(enum.Enum):
    """Enumerable for instrument category."""
    FUTURE = 0
    ETF = 1

class BaseAutoTrader:
    def __init__(self, loop: asyncio.AbstractEventLoop):
        """Initialise a new instance of the AutoTrader class."""
        self.event_loop = loop
        self.logger = logging.Logger

    def send_amend_order(self, client_order_id: int, volume: int) -> None:
        """Amend the specified order with an updated volume."""
        raise NotImplementedError

    def send_cancel_order(self, client_order_id: int) -> None:
        """Cancel the specified order."""
        raise NotImplementedError

    def send_insert_order(self, client_order_id: int, side: Side, price: int,
                          volume: int, lifespan: Lifespan) -> None:
        """Insert a new order to buy or sell the ETF."""
        raise NotImplementedError
    
    def on_error_message(self, client_order_id: int, error_message: bytes) -> None:
        """Called when the exchange detects an error."""
        pass

    def on_order_book_update_message(self, instrument: int, sequence_number: int,
                                     ask_prices: List[int], ask_volumes: List[int],
                                     bid_prices: List[int], bid_volumes: List[int]
                                    ) -> None:
        """Called periodically to report the status of an order book."""
        pass

    def on_order_status_message(self, client_order_id: int, fill_volume: int,
                                remaining_volume: int, fees: int) -> None:
        """Called when the status of one of your orders changes.
        :client_order_id: Strictly increasing id for an order"""
        pass

    def on_position_change_message(self, future_position: int, etf_position: int) -> None:
        """Called when your position changes."""
        pass

    def on_trade_ticks_message(self, instrument: int,
                               trade_ticks: List[Tuple[int, int]]) -> None:
        """Called periodically to report trading activity on the market."""
        pass