# Scheduling Questions

### Name: Anpu Li

### NetID: al4902

## Multiple Choices

1. a
2. a

## Scheduling Problem

| Date | Event(s) | Elected task | State of the queue |
|------|------------------------|--------------|--------------------------|
| 0 | Load T1.0 | T1.0 | T1.0 T2.0 T3 |
| 15 | Switch | T2.0 | T2.0 T3 |
| 115 | Switch | T3 | T3 / T2.0 |
| 150 | Insertion of T1.1 | T3 | T3 T1.1 / T2.0 |
| 225 | Switch | T1.1 | T1.1 T3 / T2.0 |
| 230 | Switch | T3 | T3 / T2.0 |
| 300 | Insertion of T1.2 T2.1 | T3 | T3 T1.2 T2.1 / T2.0 |
| 330 | Switch | T1.2 | T1.2 T2.1 T3 / T2.0 |
| 345 | Switch | T2.1 | T2.1 T3 / T2.0 |
| 445 | Switch | T3 | T3 / T2.0 T2.1 |
| 450 | Insertion of T1.3 | T3 | T3 T1.3 / T2.0 T2.1 |
| 545 | Switch | T1.3 | T1.3 T3 / T2.0 T2.1 |
| 560 | Switch | T3 | T3 / T2.0 T2.1 |
| 600 | Insertion of T1.4 T2.2 | T3 | T3 T1.4 T2.2 / T2.0 T2.1 |
| 610 | Switch | T1.4 | T1.4 T2.2 / T2.0 T2.1 |
