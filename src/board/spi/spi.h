#ifndef SRC_BOARD_SPI_SPI_H_
#define SRC_BOARD_SPI_SPI_H_

#include <src/board/board.h>
#include <src/util/data_types.h>
#include <ti/drivers/SPI.h>
#include <ti/sysbios/knl/Semaphore.h>

class Spi {
   public:
    static Spi *GetInstance();
    SPI_Handle GetHandle() const;
    SPI_Params GetSpiParams() const;
    bool PerformTransaction(uint32_t slave_select_index, byte *read_buffer,
                            byte *write_buffer, uint32_t buffer_lengths) const;
    bool PerformReadTransaction(uint32_t slave_select_index, byte *read_buffer,
                                uint32_t read_buffer_length) const;
    bool PerformWriteTransaction(uint32_t slave_select_index,
                                 byte *write_buffer,
                                 uint32_t write_buffer_length) const;

   private:
    static Spi *instance;
    static const uint32_t kTimeoutMilliSeconds = 500;
    static const uint32_t kSpiIndex = NVM;

    Spi();
    Spi(const Spi &);
    Spi &operator=(const Spi &);

    void Open();
    void Close();
    static void ManageSpiTimeout(SPI_Handle handle,
                                 SPI_Transaction *spi_transaction);
    Semaphore_Handle transaction_complete;
    SPI_Params spi_params;
    SPI_Handle handle;
};

#endif  // SRC_BOARD_SPI_SPI_H_
