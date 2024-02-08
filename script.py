import random
from datetime import datetime

# Function to generate a random FIX message
def generate_random_fix_message():
    fields = {
        "8": "FIX.4.4",
        "35": "W",
        "49": "SenderCompID",
        "52": datetime.utcnow().strftime("%Y%m%d-%H:%M:%S.%f")[:-3],
        "55": "USD/INR",
        "56": "TargetCompID",
        "262": "1",
        "268": random.randint(1, 6),
        "10": random.randint(100, 999),
    }

    fix_message = []
    for tag, value in fields.items():
        fix_message.append(f"{tag}={value}")
        if tag != "268":
            continue
        
        for repeat in range(value):
            repeating_group = {
                "270": round(random.uniform(10.0000, 14.0000), 4),
                "271": random.randint(100000, 1000000),
                "269": random.randint(0, 1),
            }
            for repeat_tag, repeat_value in repeating_group.items():
                fix_message.append(f"{repeat_tag}={repeat_value}")

    return f"{'|'.join(fix_message)}\x01"  # FIX messages are terminated by ASCII character 0x01 (SOH)

with open("messages.txt", "w") as file:
    for counter in range(1000):
        file.write(f"{generate_random_fix_message()}\n")
