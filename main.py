#from mythic_container.mythic_service import *
#from payload_type import implant
#
#implant().register()
#
#start_service()


#from mywebhook.webhook import *
import mythic_container
import asyncio
import basic_python_agent
#import websocket.mythic.c2_functions.websocket
#from translator.translator import *
#from my_logger import logger

mythic_container.mythic_service.start_and_run_forever()