#include "transf_arq.h"
#include <sys/types.h>
#include <netdb.h>

ip * cria_data_ip(){
    ip * data_ip = malloc(sizeof(ip));
    return data_ip;
}

void preenche_ip(ip *data_ip, uint32_t ip_destino){
    struct hostent *hp, *gethostbyname();

    hp = gethostbyname("localhost");
        if (hp==0){
          perror("Host nao encontrado");
          exit(2);
        }
    bcopy ((char*)hp->h_addr, (char *)&data_ip->Cabecalho_ip.end_ip_org, hp->h_length);
    data_ip->Cabecalho_ip.end_ip_dest = ip_destino;
    
}