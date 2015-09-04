/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : SPI_ADC.c
**     Project     : SeismicPi_Firmware
**     Processor   : MK22FN128VLH10
**     Component   : SPIMaster_LDD
**     Version     : Component 01.111, Driver 01.08, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-09-04, 09:33, # CodeGen: 0
**     Abstract    :
**         This component "SPIMaster_LDD" implements MASTER part of synchronous
**         serial master-slave communication.
**     Settings    :
**          Component name                                 : SPI_ADC
**          Device                                         : SPI0
**          Interrupt service/event                        : Disabled
**          Settings                                       : 
**            Input pin                                    : Enabled
**              Pin                                        : CMP0_IN1/PTC7/SPI0_SIN/USB_SOF_OUT/I2S0_RX_FS
**            Output pin                                   : Enabled
**              Pin                                        : CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/I2S0_MCLK
**            Clock pin                                    : 
**              Pin                                        : PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/CMP0_OUT/FTM0_CH2
**            Chip select list                             : 0
**            CS external demultiplexer                    : Disabled
**            Attribute set list                           : 1
**              Attribute set 0                            : 
**                Width                                    : 8 bits
**                MSB first                                : yes
**                Clock polarity                           : Low
**                Clock phase                              : Capture on leading edge
**                Parity                                   : None
**                Chip select toggling                     : no
**                Clock rate index                         : 0
**                Delay after transfer index               : 0
**                CS to CLK delay index                    : 0
**                CLK to CS delay index                    : 0
**            Clock rate                                   : 23.994368 MHz
**            Delay after transfer                         : 0.041676 �s
**            CS to CLK delay                              : 0.041676 �s
**            CLK to CS delay                              : 0.041676 �s
**            HW input buffer size                         : 1
**            HW input watermark                           : 1
**            HW output buffer size                        : 1
**            HW output watermark                          : 1
**          Initialization                                 : 
**            Initial chip select                          : 0
**            Initial attribute set                        : 0
**            Enabled in init. code                        : yes
**            Auto initialization                          : no
**            Event mask                                   : 
**              OnBlockSent                                : Enabled
**              OnBlockReceived                            : Enabled
**              OnError                                    : Disabled
**          CPU clock/configuration selection              : 
**            Clock configuration 0                        : This component enabled
**            Clock configuration 1                        : This component disabled
**            Clock configuration 2                        : This component disabled
**            Clock configuration 3                        : This component disabled
**            Clock configuration 4                        : This component disabled
**            Clock configuration 5                        : This component disabled
**            Clock configuration 6                        : This component disabled
**            Clock configuration 7                        : This component disabled
**     Contents    :
**         Init         - LDD_TDeviceData* SPI_ADC_Init(LDD_TUserData *UserDataPtr);
**         SendBlock    - LDD_TError SPI_ADC_SendBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData...
**         ReceiveBlock - LDD_TError SPI_ADC_ReceiveBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData...
**         Main         - void SPI_ADC_Main(LDD_TDeviceData *DeviceDataPtr);
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file SPI_ADC.c
** @version 01.08
** @brief
**         This component "SPIMaster_LDD" implements MASTER part of synchronous
**         serial master-slave communication.
*/         
/*!
**  @addtogroup SPI_ADC_module SPI_ADC module documentation
**  @{
*/         

/* MODULE SPI_ADC. */
/*lint -save  -e926 -e927 -e928 -e929 Disable MISRA rule (11.4) checking. */

#include "Events.h"
#include "SPI_ADC.h"
/* {Default RTOS Adapter} No RTOS includes */

#ifdef __cplusplus
extern "C" {
#endif 

#define AVAILABLE_EVENTS_MASK (LDD_SPIMASTER_ON_BLOCK_RECEIVED | LDD_SPIMASTER_ON_BLOCK_SENT)

/* These constants contain pins masks */
#define SPI_ADC_AVAILABLE_PIN_MASK (LDD_SPIMASTER_INPUT_PIN | LDD_SPIMASTER_OUTPUT_PIN | LDD_SPIMASTER_CLK_PIN)

typedef struct {
  bool EnMode;                         /* Enable/Disable device in clock configuration */
  uint32_t TxCommand;                  /* Current Tx command */
  LDD_SPIMASTER_TError ErrFlag;        /* Error flags */
  uint16_t InpRecvDataNum;             /* The counter of received characters */
  uint8_t *InpDataPtr;                 /* The buffer pointer for received characters */
  uint16_t InpDataNumReq;              /* The counter of characters to receive by ReceiveBlock() */
  uint16_t OutSentDataNum;             /* The counter of sent characters */
  uint8_t *OutDataPtr;                 /* The buffer pointer for data to be transmitted */
  uint16_t OutDataNumReq;              /* The counter of characters to be send by SendBlock() */
  LDD_TUserData *UserData;             /* User device data structure */
} SPI_ADC_TDeviceData;                 /* Device data structure type */

typedef SPI_ADC_TDeviceData* SPI_ADC_TDeviceDataPtr ; /* Pointer to the device data structure. */

/* {Default RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static SPI_ADC_TDeviceData DeviceDataPrv__DEFAULT_RTOS_ALLOC;
/* Internal method prototypes */
static void HWEnDi(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  SPI_ADC_Init (component SPIMaster_LDD)
*/
/*!
**     @brief
**         Initializes the device. Allocates memory for the device data
**         structure, allocates interrupt vectors and sets interrupt
**         priority, sets pin routing, sets timing, etc.
**         If the "Enable in init. code" is set to "yes" value then the
**         device is also enabled(see the description of the Enable()
**         method). In this case the Enable() method is not necessary
**         and needn't to be generated. 
**         This method can be called only once. Before the second call
**         of Init() the Deinit() must be called first.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Device data structure pointer.
*/
/* ===================================================================*/
LDD_TDeviceData* SPI_ADC_Init(LDD_TUserData *UserDataPtr)
{
  /* Allocate LDD device structure */
  SPI_ADC_TDeviceDataPtr DeviceDataPrv;

  /* {Default RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DeviceDataPrv = &DeviceDataPrv__DEFAULT_RTOS_ALLOC;
  DeviceDataPrv->UserData = UserDataPtr; /* Store the RTOS device structure */
  DeviceDataPrv->TxCommand = 0x80000000U; /* Initialization of current Tx command */
  DeviceDataPrv->ErrFlag = 0x00U;      /* Clear error flags */
  /* Clear the receive counters and pointer */
  DeviceDataPrv->InpRecvDataNum = 0x00U; /* Clear the counter of received characters */
  DeviceDataPrv->InpDataNumReq = 0x00U; /* Clear the counter of characters to receive by ReceiveBlock() */
  DeviceDataPrv->InpDataPtr = NULL;    /* Clear the buffer pointer for received characters */
  /* Clear the transmit counters and pointer */
  DeviceDataPrv->OutSentDataNum = 0x00U; /* Clear the counter of sent characters */
  DeviceDataPrv->OutDataNumReq = 0x00U; /* Clear the counter of characters to be send by SendBlock() */
  DeviceDataPrv->OutDataPtr = NULL;    /* Clear the buffer pointer for data to be transmitted */
  /* SIM_SCGC6: SPI0=1 */
  SIM_SCGC6 |= SIM_SCGC6_SPI0_MASK;
  /* SIM_SCGC5: PORTC=1 */
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
  /* PORTC_PCR7: ISF=0,MUX=2 */
  PORTC_PCR7 = (uint32_t)((PORTC_PCR7 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x05)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x02)
               ));
  /* PORTC_PCR6: ISF=0,MUX=2 */
  PORTC_PCR6 = (uint32_t)((PORTC_PCR6 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x05)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x02)
               ));
  /* PORTC_PCR5: ISF=0,MUX=2 */
  PORTC_PCR5 = (uint32_t)((PORTC_PCR5 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x05)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x02)
               ));
  /* SPI0_MCR: MSTR=0,CONT_SCKE=0,DCONF=0,FRZ=0,MTFE=0,PCSSE=0,ROOE=1,??=0,??=0,PCSIS=0,DOZE=0,MDIS=0,DIS_TXF=0,DIS_RXF=0,CLR_TXF=0,CLR_RXF=0,SMPL_PT=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,HALT=1 */
  SPI0_MCR = SPI_MCR_DCONF(0x00) |
             SPI_MCR_ROOE_MASK |
             SPI_MCR_PCSIS(0x00) |
             SPI_MCR_SMPL_PT(0x00) |
             SPI_MCR_HALT_MASK;        /* Set Configuration register */
  /* SPI0_MCR: MSTR=1,CONT_SCKE=0,DCONF=0,FRZ=0,MTFE=0,PCSSE=0,ROOE=1,??=0,??=0,PCSIS=0,DOZE=0,MDIS=0,DIS_TXF=1,DIS_RXF=1,CLR_TXF=1,CLR_RXF=1,SMPL_PT=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,HALT=1 */
  SPI0_MCR = SPI_MCR_MSTR_MASK |
             SPI_MCR_DCONF(0x00) |
             SPI_MCR_ROOE_MASK |
             SPI_MCR_PCSIS(0x00) |
             SPI_MCR_DIS_TXF_MASK |
             SPI_MCR_DIS_RXF_MASK |
             SPI_MCR_CLR_TXF_MASK |
             SPI_MCR_CLR_RXF_MASK |
             SPI_MCR_SMPL_PT(0x00) |
             SPI_MCR_HALT_MASK;        /* Set Configuration register */
  /* SPI0_CTAR0: DBR=1,FMSZ=7,CPOL=0,CPHA=0,LSBFE=0,PCSSCK=0,PASC=0,PDT=0,PBR=0,CSSCK=0,ASC=0,DT=0,BR=0 */
  SPI0_CTAR0 = SPI_CTAR_DBR_MASK |
               SPI_CTAR_FMSZ(0x07) |
               SPI_CTAR_PCSSCK(0x00) |
               SPI_CTAR_PASC(0x00) |
               SPI_CTAR_PDT(0x00) |
               SPI_CTAR_PBR(0x00) |
               SPI_CTAR_CSSCK(0x00) |
               SPI_CTAR_ASC(0x00) |
               SPI_CTAR_DT(0x00) |
               SPI_CTAR_BR(0x00);      /* Set Clock and Transfer Attributes register */
  /* SPI0_SR: TCF=1,TXRXS=0,??=0,EOQF=1,TFUF=1,??=0,TFFF=1,??=0,??=0,??=0,??=1,??=0,RFOF=1,??=0,RFDF=1,??=0,TXCTR=0,TXNXTPTR=0,RXCTR=0,POPNXTPTR=0 */
  SPI0_SR = SPI_SR_TCF_MASK |
            SPI_SR_EOQF_MASK |
            SPI_SR_TFUF_MASK |
            SPI_SR_TFFF_MASK |
            SPI_SR_RFOF_MASK |
            SPI_SR_RFDF_MASK |
            SPI_SR_TXCTR(0x00) |
            SPI_SR_TXNXTPTR(0x00) |
            SPI_SR_RXCTR(0x00) |
            SPI_SR_POPNXTPTR(0x00) |
            0x00200000U;               /* Clear flags */
  /* SPI0_RSER: TCF_RE=0,??=0,??=0,EOQF_RE=0,TFUF_RE=0,??=0,TFFF_RE=0,TFFF_DIRS=0,??=0,??=0,??=0,??=0,RFOF_RE=0,??=0,RFDF_RE=0,RFDF_DIRS=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  SPI0_RSER = 0x00U;                   /* Set DMA Interrupt Request Select and Enable register */
  SPI_ADC_SetClockConfiguration(DeviceDataPrv, Cpu_GetClockConfiguration()); /* Set Initial according speed CPU mode */
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_SPI_ADC_ID,DeviceDataPrv);
  return ((LDD_TDeviceData *)DeviceDataPrv); /* Return pointer to the data data structure */
}

/*
** ===================================================================
**     Method      :  SPI_ADC_ReceiveBlock (component SPIMaster_LDD)
*/
/*!
**     @brief
**         This method specifies the number of data to receive. The
**         method returns ERR_BUSY until the specified number of
**         characters is received. The method [CancelBlockReception]
**         can be used to cancel a running receive operation. If a
**         receive operation is not in progress (the method was not
**         called or a previous operation has already finished) all
**         received characters will be lost without any notification.
**         To prevent the loss of data call the method immediately
**         after the last receive operation has finished (e.g. from the
**         [OnBlockReceived] event). This method finishes immediately
**         after calling it - it doesn't wait the end of data reception.
**         Use event [OnBlockReceived] to check the end of data
**         reception or method GetReceivedDataNum to check the state of
**         receiving.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         BufferPtr       - Pointer to A buffer where
**                           received characters will be stored.
**     @param
**         Size            - Size of the block
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_DISABLED - Component is disabled
**                           ERR_BUSY - The previous receive request is
**                           pending
*/
/* ===================================================================*/
LDD_TError SPI_ADC_ReceiveBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData *BufferPtr, uint16_t Size)
{
  /* Clock configuration test - this test can be disabled by setting the "Ignore clock configuration test"
     property to the "yes" value in the "Configuration inspector" */
  if (!((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->EnMode) { /* Is the device disabled in the actual speed CPU mode? */
    return ERR_SPEED;                  /* If yes then error */
  }
  if (((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->InpDataNumReq != 0x00U) { /* Is the previous receive operation pending? */
    return ERR_BUSY;                   /* If yes then error */
  }
  /* {Default RTOS Adapter} Critical section begin, general PE function is used */
  EnterCritical();
  ((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->InpDataPtr = (uint8_t*)BufferPtr; /* Store a pointer to the input data. */
  ((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->InpDataNumReq = Size; /* Store a number of characters to be received. */
  ((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->InpRecvDataNum = 0x00U; /* Set number of received characters to zero. */
  /* {Default RTOS Adapter} Critical section end, general PE function is used */
  ExitCritical();
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SPI_ADC_SendBlock (component SPIMaster_LDD)
*/
/*!
**     @brief
**         Sends a block of characters. The method returns ERR_BUSY
**         when the previous block transmission is not completed. The
**         method [CancelBlockTransmission] can be used to cancel a
**         transmit operation. This method finishes immediately after
**         calling it - it doesn't wait the end of data transmission.
**         Use event [OnBlockSent] to check the end of data
**         transmission or method GetSentDataNum to check the state of
**         sending.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         BufferPtr       - Pointer to the block of data
**                           to send.
**     @param
**         Size            - Number of characters in the buffer.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_DISABLED - Component is disabled
**                           ERR_BUSY - The previous transmit request is
**                           pending
*/
/* ===================================================================*/
LDD_TError SPI_ADC_SendBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData *BufferPtr, uint16_t Size)
{
  /* Clock configuration test - this test can be disabled by setting the "Ignore clock configuration test"
     property to the "yes" value in the "Configuration inspector" */
  if (!((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->EnMode) { /* Is the device disabled in the actual speed CPU mode? */
    return ERR_SPEED;                  /* If yes then error */
  }
  if (((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->OutDataNumReq != 0x00U) { /* Is the previous transmit operation pending? */
    return ERR_BUSY;                   /* If yes then error */
  }
  /* {Default RTOS Adapter} Critical section begin, general PE function is used */
  EnterCritical();
  ((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->OutDataPtr = (uint8_t*)BufferPtr; /* Set a pointer to the output data. */
  ((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->OutDataNumReq = Size; /* Set the counter of characters to be sent. */
  ((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->OutSentDataNum = 0x00U; /* Clear the counter of sent characters. */
  /* {Default RTOS Adapter} Critical section end, general PE function is used */
  ExitCritical();
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SPI_ADC_Main (component SPIMaster_LDD)
*/
/*!
**     @brief
**         This method is available only in the polling mode (Interrupt
**         service/event = 'no'). If interrupt service is disabled this
**         method replaces the interrupt handler. This method should be
**         called if Receive/SendBlock was invoked before in order to
**         run the reception/transmission. The end of the
**         receiving/transmitting is indicated by OnBlockSent or
**         OnBlockReceived event. 
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
*/
/* ===================================================================*/
void SPI_ADC_Main(LDD_TDeviceData *DeviceDataPtr)
{
  SPI_ADC_TDeviceDataPtr DeviceDataPrv = (SPI_ADC_TDeviceDataPtr)DeviceDataPtr;
  uint32_t StatReg = SPI_PDD_GetInterruptFlags(SPI0_BASE_PTR); /* Read status register */
  uint16_t Data;                       /* Temporary variable for data */

  /* Clock configuration test - this test can be disabled by setting the "Ignore clock configuration test"
     property to the "yes" value in the "Configuration inspector" */
  if (!DeviceDataPrv->EnMode) {        /* Is the device disabled by user? */
    return;                            /* If yes then exit */
  }
  if ((StatReg & SPI_PDD_RX_FIFO_OVERFLOW_INT) != 0U) { /* Is any error flag set? */
    SPI_PDD_ClearInterruptFlags(SPI0_BASE_PTR,SPI_PDD_RX_FIFO_OVERFLOW_INT); /* Clear error flags */
  }
  if ((StatReg & SPI_PDD_RX_FIFO_DRAIN_INT_DMA) != 0U) { /* Is any char in HW buffer? */
    Data = (uint16_t)SPI_PDD_ReadPopRxFIFOReg(SPI0_BASE_PTR); /* Read character from receiver */
    SPI_PDD_ClearInterruptFlags(SPI0_BASE_PTR,SPI_PDD_RX_FIFO_DRAIN_INT_DMA); /* Clear Rx FIFO drain flags */
    if (DeviceDataPrv->InpDataNumReq != 0x00U) { /* Is the receive block operation pending? */
      *(DeviceDataPrv->InpDataPtr++) = (uint8_t)Data; /* Put a character to the receive buffer and increment pointer to receive buffer */
      DeviceDataPrv->InpRecvDataNum++; /* Increment received char. counter */
      if (DeviceDataPrv->InpRecvDataNum == DeviceDataPrv->InpDataNumReq) { /* Is the requested number of characters received? */
        DeviceDataPrv->InpDataNumReq = 0x00U; /* If yes then clear number of requested characters to be received. */
        SPI_ADC_OnBlockReceived(DeviceDataPrv->UserData);
      }
    }
  }
  if ((StatReg & SPI_PDD_TX_FIFO_FILL_INT_DMA) != 0U) { /* Is HW buffer empty? */
    if (DeviceDataPrv->OutSentDataNum < DeviceDataPrv->OutDataNumReq) { /* Is number of sent characters less than the number of requested incoming characters? */
      DeviceDataPrv->OutSentDataNum++; /* Increment the counter of sent characters. */
      SPI_PDD_WriteMasterPushTxFIFOReg(SPI0_BASE_PTR, (uint32_t)(*((uint8_t *)DeviceDataPrv->OutDataPtr++) | DeviceDataPrv->TxCommand)); /* Put a character with command to the transmit register and increment pointer to the transmitt buffer */
      if (DeviceDataPrv->OutSentDataNum == DeviceDataPrv->OutDataNumReq) {
        DeviceDataPrv->OutDataNumReq = 0x00U; /* Clear the counter of characters to be send by SendBlock() */
        SPI_ADC_OnBlockSent(DeviceDataPrv->UserData);
      }
    } else {
    }
    SPI_PDD_ClearInterruptFlags(SPI0_BASE_PTR,SPI_PDD_TX_FIFO_FILL_INT_DMA); /* Clear Tx FIFO fill flags */
  }
}

/*
** ===================================================================
**     Method      :  HWEnDi (component SPIMaster_LDD)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the 
**         component. The method is called automatically as a part of the 
**         Enable and Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void HWEnDi(LDD_TDeviceData *DeviceDataPtr)
{
  if (((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->EnMode) { /* Enable device? */
    ((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->OutDataNumReq = 0x00U; /* Clear the counter of requested outgoing characters. */
    ((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->OutSentDataNum = 0x00U; /* Clear the counter of sent characters. */
    ((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->InpDataNumReq = 0x00U; /* Clear the counter of requested incoming characters. */
    ((SPI_ADC_TDeviceDataPtr)DeviceDataPtr)->InpRecvDataNum = 0x00U; /* Clear the counter of received characters. */
    SPI_PDD_EnableDevice(SPI0_BASE_PTR,PDD_ENABLE); /* Enable device */
    SPI_PDD_ClearTxFIFO(SPI0_BASE_PTR); /* Clear Tx FIFO */
    SPI_PDD_ClearRxFIFO(SPI0_BASE_PTR); /* Clear Rx FIFO */
    SPI_PDD_ClearInterruptFlags(SPI0_BASE_PTR,SPI_PDD_ALL_INT); /* Clear all HW flags */
    SPI_PDD_EnableHaltMode(SPI0_BASE_PTR,PDD_DISABLE); /* Leave the STOPPED mode */
  }
  else {
    SPI_PDD_EnableHaltMode(SPI0_BASE_PTR,PDD_ENABLE); /* Enter to STOPPED mode */
    SPI_PDD_EnableDevice(SPI0_BASE_PTR,PDD_DISABLE); /* Disable device */
  }
}

/*
** ===================================================================
**     Method      :  SPI_ADC_SetClockConfiguration (component SPIMaster_LDD)
**
**     Description :
**         This method changes the clock configuration. During a clock 
**         configuration change the component changes it`s setting 
**         immediately upon a request.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void SPI_ADC_SetClockConfiguration(LDD_TDeviceData *DeviceDataPtr, LDD_TClockConfiguration ClockConfiguration)
{
  SPI_ADC_TDeviceData *DeviceDataPrv = (SPI_ADC_TDeviceData *)DeviceDataPtr;

  switch (ClockConfiguration) {
    case CPU_CLOCK_CONFIG_0:
      DeviceDataPrv->EnMode = TRUE;    /* Set the flag "device enabled" in the actual speed CPU mode */
      SPI_PDD_WriteMasterClockTransferAttributeReg(SPI0_BASE_PTR,0U,0xB8000000U); /* Set required configuration */
      break;
    default:
      DeviceDataPrv->EnMode = FALSE;   /* Set the flag "device disabled" in the actual speed CPU mode */
      break;
  }
  HWEnDi(DeviceDataPtr);               /* Enable/disable device according to status flags */
}

/*lint -restore Enable MISRA rule (11.4) checking. */
/* END SPI_ADC. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/