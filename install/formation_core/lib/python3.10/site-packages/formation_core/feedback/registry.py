from __future__ import annotations
from .zero import ZeroFeedback
from .consensus import ConsensusFeedback


def build_feedback(cfg: dict):
    """cfg: {type: zero}  or  {type: consensus, gain: 0.8, max_correction: 1.5}"""
    t = (cfg or {}).get('type', 'zero')
    if t == 'zero':
        return ZeroFeedback()
    if t == 'consensus':
        return ConsensusFeedback(gain=float(cfg.get('gain', 0.8)),
                                 max_correction=float(cfg.get('max_correction', 1.5)))
    raise ValueError(f"unknown feedback type: {t}")
