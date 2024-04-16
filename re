using Microsoft.AspNetCore.Mvc;
using System;
using System.Net.Http;
using System.Threading.Tasks;

namespace TranslationAPI.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class TranslationController : ControllerBase
    {
        private readonly HttpClient _httpClient; // Instância do HttpClient para fazer solicitações HTTP.

        public TranslationController()
        {
            // Configuração do HttpClient com a chave de assinatura para autenticação.
            _httpClient = new HttpClient();
            _httpClient.DefaultRequestHeaders.Add("Ocp-Apim-Subscription-Key", "SUA_CHAVE_DE_ASSINATURA_AQUI");
        }

        [HttpPost("TextToSpeech")]
        public async Task<IActionResult> TextToSpeech([FromBody] string text)
        {
            try
            {
                // Faz uma solicitação POST para a API de conversão de texto para fala da Azure.
                var response = await _httpClient.PostAsync("https://westus.tts.speech.microsoft.com/cognitiveservices/v1",
                    new StringContent($"{{\"text\":\"{text}\"}}"));
                
                response.EnsureSuccessStatusCode(); // Verifica se a solicitação foi bem-sucedida.

                // Retorna o áudio da resposta como um arquivo WAV.
                return File(await response.Content.ReadAsStreamAsync(), "audio/wav");
            }
            catch (Exception ex)
            {
                // Retorna um erro em caso de falha na solicitação.
                return BadRequest($"Error: {ex.Message}");
            }
        }

        [HttpPost("SpeechToText")]
        public async Task<IActionResult> SpeechToText([FromBody] byte[] audio)
        {
            try
            {
                // Faz uma solicitação POST para a API de conversão de fala para texto da Azure.
                var content = new ByteArrayContent(audio) { Headers = { { "Content-Type", "audio/wav" } } };
                var response = await _httpClient.PostAsync("https://westus.stt.speech.microsoft.com/speech/recognition/conversation/cognitiveservices/v1", content);
                
                response.EnsureSuccessStatusCode(); // Verifica se a solicitação foi bem-sucedida.

                // Retorna o texto da resposta.
                return Ok(await response.Content.ReadAsStringAsync());
            }
            catch (Exception ex)
            {
                // Retorna um erro em caso de falha na solicitação.
                return BadRequest($"Error: {ex.Message}");
            }
        }
    }
}
